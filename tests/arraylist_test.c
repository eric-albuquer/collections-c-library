#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "collections.h"

// --- FUNÇÕES AUXILIARES PARA TESTES ---

// Comparador para inteiros em ordem crescente (usado no Sort)
int int_asc_compare(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

// Acumulador global para testar a função Map
static int g_map_sum = 0;
static int g_map_calls = 0;

void accumulate_map_func(int idx, const void* element) {
    (void)idx;
    g_map_sum += *(const int*)element;
    g_map_calls++;
}

// --- TESTES ---

// Teste 1: Criação, Capacidade Inicial e Operações Básicas (Append/Pop)
void test_arraylist_creation_and_basic(void) {
    // 1. Criação com capacidade automática (0)
    ArrayList list = arraylist_create(sizeof(int), 0);
    assert(list._size == 0);
    assert(list._element_size == sizeof(int));
    assert(arraylist_length(&list) == 0);

    int v1 = 10, v2 = 20, v3 = 30;

    arraylist_append(&list, &v1);
    arraylist_append(&list, &v2);
    arraylist_append(&list, &v3);
    assert(arraylist_length(&list) == 3);

    // 2. Remoção LIFO (pop)
    int out = 0;
    assert(arraylist_pop(&list, &out) == true);
    assert(out == 30);
    assert(arraylist_length(&list) == 2);

    assert(arraylist_pop(&list, &out) == true);
    assert(out == 20);

    assert(arraylist_pop(&list, &out) == true);
    assert(out == 10);
    assert(arraylist_length(&list) == 0);

    // Pop em lista vazia deve retornar false (underflow seguro)
    assert(arraylist_pop(&list, &out) == false);

    arraylist_free(&list);

    // 3. Criação com tamanho inicial customizado
    ArrayList custom_list = arraylist_create(sizeof(int), 50);
    assert(custom_list._capacity >= 50);
    assert(arraylist_length(&custom_list) == 0);
    arraylist_free(&custom_list);

    printf("[OK] Teste de criacao e operacoes basicas concluido.\n");
}

// Teste 2: Acesso e Modificação Posicional (Get e Set)
void test_arraylist_get_set(void) {
    ArrayList list = arraylist_create(sizeof(int), 0);

    int values[] = {100, 200, 300};
    for (int i = 0; i < 3; i++) {
        arraylist_append(&list, &values[i]);
    }

    // 1. Get válido
    int out = 0;
    assert(arraylist_get(&list, 0, &out) == true);
    assert(out == 100);

    assert(arraylist_get(&list, 2, &out) == true);
    assert(out == 300);

    // 2. Get fora dos limites (Out of bounds)
    assert(arraylist_get(&list, -1, &out) == false);
    assert(arraylist_get(&list, 3, &out) == false);

    // 3. Set válido
    int new_val = 999;
    assert(arraylist_set(&list, 1, &new_val) == true); // Altera índice 1 (200 -> 999)
    assert(arraylist_get(&list, 1, &out) == true);
    assert(out == 999);

    // 4. Set fora dos limites
    assert(arraylist_set(&list, -1, &new_val) == false);
    assert(arraylist_set(&list, 10, &new_val) == false);

    arraylist_free(&list);
    printf("[OK] Teste de Get e Set concluido.\n");
}

// Teste 3: Inserção e Remoção por Índice Específico
void test_arraylist_insert_remove_index(void) {
    ArrayList list = arraylist_create(sizeof(int), 0);

    int a = 1, b = 3, c = 2;
    arraylist_append(&list, &a); // [1]
    arraylist_append(&list, &b); // [1, 3]

    // 1. Inserir no meio (índice 1) -> [1, 2, 3]
    assert(arraylist_insert_index(&list, 1, &c) == true);
    assert(arraylist_length(&list) == 3);
    
    int out = 0;
    arraylist_get(&list, 1, &out);
    assert(out == 2);

    // 2. Inserir fora dos limites
    int invalid = 99;
    assert(arraylist_insert_index(&list, -1, &invalid) == false);
    assert(arraylist_insert_index(&list, 10, &invalid) == false);

    // 3. Remover do índice 1 -> [1, 3]
    assert(arraylist_remove_index(&list, 1, &out) == true);
    assert(out == 2);
    assert(arraylist_length(&list) == 2);

    arraylist_get(&list, 1, &out);
    assert(out == 3);

    // 4. Remover fora dos limites
    assert(arraylist_remove_index(&list, -1, &out) == false);
    assert(arraylist_remove_index(&list, 5, &out) == false);

    arraylist_free(&list);
    printf("[OK] Teste de insercao e remocao por indice concluido.\n");
}

// Teste 4: Swap, Clear e Verificações de Estado
void test_arraylist_swap_and_clear(void) {
    ArrayList list = arraylist_create(sizeof(int), 0);
    int x = 10, y = 20;

    arraylist_append(&list, &x);
    arraylist_append(&list, &y);

    // 1. Teste de Swap
    assert(arraylist_swap(&list, 0, 1) == true);
    int out = 0;
    arraylist_get(&list, 0, &out);
    assert(out == 20);
    arraylist_get(&list, 1, &out);
    assert(out == 10);

    // Swap com índices inválidos
    assert(arraylist_swap(&list, 0, 5) == false);

    // 2. Teste de Clear
    arraylist_clear(&list);
    assert(arraylist_length(&list) == 0);
    
    // Tentar dar get após clear deve falhar
    assert(arraylist_get(&list, 0, &out) == false);

    arraylist_free(&list);
    printf("[OK] Teste de swap e clear concluido.\n");
}

// Teste 5: Ordenação (Sort), Mapeamento (Map) e Embaralhamento (Shuffle)
void test_arraylist_sort_map_shuffle(void) {
    ArrayList list = arraylist_create(sizeof(int), 0);

    int unsorted[] = {42, 12, 88, 3, 25};
    int n = sizeof(unsorted) / sizeof(unsorted[0]);

    for (int i = 0; i < n; i++) {
        arraylist_append(&list, &unsorted[i]);
    }

    // 1. Teste de Ordenação
    arraylist_sort(&list, int_asc_compare);
    int expected_sorted[] = {3, 12, 25, 42, 88};
    for (int i = 0; i < n; i++) {
        int out = 0;
        arraylist_get(&list, i, &out);
        assert(out == expected_sorted[i]);
    }

    // 2. Teste do Map
    g_map_sum = 0;
    g_map_calls = 0;
    arraylist_foreach(&list, accumulate_map_func);
    assert(g_map_calls == n);
    assert(g_map_sum == (3 + 12 + 25 + 42 + 88));

    // 3. Teste de Shuffle (verifica se não quebra o tamanho e mantém os elementos)
    int size_before = arraylist_length(&list);
    arraylist_shuffle(&list);
    assert(arraylist_length(&list) == size_before);

    arraylist_free(&list);
    printf("[OK] Teste de sort, map e shuffle concluido.\n");
}

// Teste 6: Stress e Performance (Benchmark)
void test_arraylist_performance_and_stress(void) {
    printf("[BENCHMARK] Iniciando teste de estresse e performance para ArrayList...\n");

    clock_t start = clock();
    
    ArrayList list = arraylist_create(sizeof(int), 0);
    int total = 50000;

    // Inserção em massa (testando redimensionamento dinâmico contínuo)
    for (int i = 0; i < total; i++) {
        arraylist_append(&list, &i);
    }

    assert(arraylist_length(&list) == total);

    // Acesso posicional em massa
    for (int i = 0; i < total; i++) {
        int out = 0;
        assert(arraylist_get(&list, i, &out) == true);
        assert(out == i);
    }

    // Ordenação de muitos elementos
    clock_t sort_start = clock();
    arraylist_sort(&list, int_asc_compare);
    clock_t sort_end = clock();

    arraylist_free(&list);

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;
    double sort_time = (double)(sort_end - sort_start) / CLOCKS_PER_SEC;

    printf(" -> Total de elementos: %d\n", total);
    printf(" -> Tempo total (append + get + sort): %.6f segundos\n", total_time);
    printf(" -> Tempo dedicado apenas ao Sort: %.6f segundos\n", sort_time);
    printf("[OK] Teste de performance e estresse do ArrayList concluido!\n");
}

int main(void) {
    printf("--- Executando testes robustos para ArrayList ---\n\n");

    test_arraylist_creation_and_basic();
    test_arraylist_get_set();
    test_arraylist_insert_remove_index();
    test_arraylist_swap_and_clear();
    test_arraylist_sort_map_shuffle();
    test_arraylist_performance_and_stress();

    printf("\n--- Todos os testes robustos do ArrayList passaram com sucesso! 🎉 ---\n");
    return 0;
}