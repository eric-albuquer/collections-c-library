#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "collections.h"

// --- FUNÇÕES AUXILIARES DE COMPARAÇÃO E MAP ---

// Comparador para inteiros em ordem crescente
int int_asc_cmp(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    return arg1 - arg2;
}

// Acumulador global simples para validar a execução do arraylist_map
static int g_map_sum = 0;
static int g_map_call_count = 0;

void sum_map_func(int idx, const void* element) {
    (void)idx; // Evita warning de variável não utilizada
    int val = *(const int*)element;
    g_map_sum += val;
    g_map_call_count++;
}

// --- TESTES ---

// Teste 1: Criação com capacidade automática vs personalizada e Append/Pop
void test_arraylist_basic_ops(void) {
    // 1. Teste de capacidade automática (0)
    ArrayList list = arraylist_create(sizeof(int), 0);
    assert(list._size == 0);
    assert(list._element_size == sizeof(int));

    int v1 = 10, v2 = 20, v3 = 30;

    arraylist_append(&list, &v1);
    arraylist_append(&list, &v2);
    arraylist_append(&list, &v3);
    assert(list._size == 3);

    // 2. Remoção LIFO (pop)
    int out = 0;
    assert(arraylist_pop(&list, &out) == 1);
    assert(out == 30);
    assert(list._size == 2);

    assert(arraylist_pop(&list, &out) == 1);
    assert(out == 20);

    assert(arraylist_pop(&list, &out) == 1);
    assert(out == 10);
    assert(list._size == 0);

    // Underflow (pop em lista vazia)
    assert(arraylist_pop(&list, &out) == 0);

    arraylist_free(list);

    // 3. Teste de criação com tamanho inicial customizado
    ArrayList custom_list = arraylist_create(sizeof(int), 50);
    assert(custom_list._capacity >= 50);
    arraylist_free(custom_list);

    printf("[OK] Teste de operacoes basicas e criacao concluido.\n");
}

// Teste 2: Acesso e Alteração Posicional (Get e Set)
void test_arraylist_get_set(void) {
    ArrayList list = arraylist_create(sizeof(int), 0);

    int values[] = {100, 200, 300};
    for (int i = 0; i < 3; i++) {
        arraylist_append(&list, &values[i]);
    }

    // 1. Get válido
    int out = 0;
    assert(arraylist_get(list, 0, &out) == 1);
    assert(out == 100);

    assert(arraylist_get(list, 2, &out) == 1);
    assert(out == 300);

    // 2. Get fora dos limites (Out of bounds)
    assert(arraylist_get(list, -1, &out) == 0);
    assert(arraylist_get(list, 3, &out) == 0);

    // 3. Set válido
    int new_val = 999;
    assert(arraylist_set(&list, 1, &new_val) == 1); // Altera o elemento no índice 1 (200 -> 999)

    assert(arraylist_get(list, 1, &out) == 1);
    assert(out == 999);

    // 4. Set fora dos limites
    assert(arraylist_set(&list, -1, &new_val) == 0);
    assert(arraylist_set(&list, 10, &new_val) == 0);

    arraylist_free(list);
    printf("[OK] Teste de Get e Set concluido.\n");
}

// Teste 3: Ordenação (Sort) e Mapeamento/Iteração (Map)
void test_arraylist_sort_and_map(void) {
    ArrayList list = arraylist_create(sizeof(int), 0);

    int unsorted[] = {42, 12, 88, 3, 25};
    int n = sizeof(unsorted) / sizeof(unsorted[0]);

    for (int i = 0; i < n; i++) {
        arraylist_append(&list, &unsorted[i]);
    }

    // 1. Ordenação
    arraylist_sort(list, int_asc_cmp);

    int expected_sorted[] = {3, 12, 25, 42, 88};
    for (int i = 0; i < n; i++) {
        int out = 0;
        assert(arraylist_get(list, i, &out) == 1);
        assert(out == expected_sorted[i]);
    }

    // 2. Teste do Map
    g_map_sum = 0;
    g_map_call_count = 0;

    arraylist_map(list, sum_map_func);

    assert(g_map_call_count == n);
    assert(g_map_sum == (3 + 12 + 25 + 42 + 88)); // Soma total dos elementos

    arraylist_free(list);
    printf("[OK] Teste de Sort e Map concluido.\n");
}

// Teste 4: Carga e Redimensionamento Dinâmico (Stress Test)
void test_arraylist_stress(void) {
    ArrayList list = arraylist_create(sizeof(int), 0);
    int total = 1000;

    for (int i = 0; i < total; i++) {
        arraylist_append(&list, &i);
    }

    assert(list._size == total);

    for (int i = 0; i < total; i++) {
        int out = 0;
        assert(arraylist_get(list, i, &out) == 1);
        assert(out == i);
    }

    arraylist_free(list);
    printf("[OK] Teste de carga e redimensionamento concluido.\n");
}

int main(void) {
    printf("--- Executando testes para ArrayList ---\n");

    test_arraylist_basic_ops();
    test_arraylist_get_set();
    test_arraylist_sort_and_map();
    test_arraylist_stress();

    printf("--- Todos os testes passaram com sucesso! ---\n");
    return 0;
}