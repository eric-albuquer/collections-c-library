#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "collections.h"

// --- VARIÁVEIS GLOBAIS AUXILIARES PARA TESTES ---
static int g_foreach_sum = 0;
static int g_foreach_count = 0;

void accumulate_foreach_func(const void* element) {
    g_foreach_sum += *(const int*)element;
    g_foreach_count++;
}

// --- TESTES ---

// Teste 1: Criação e Operações Básicas (Add, Has, Length, Remove)
void test_hashset_basic(void) {
    HashSet set = hashset_create(sizeof(int), int_equals, int_hash);
    assert(hashset_length(&set) == 0);

    int v1 = 10, v2 = 20, v3 = 30;

    // 1. Adição de elementos
    hashset_add(&set, &v1);
    hashset_add(&set, &v2);
    hashset_add(&set, &v3);
    assert(hashset_length(&set) == 3);

    // Adição de duplicata (não deve alterar o tamanho)
    hashset_add(&set, &v1);
    assert(hashset_length(&set) == 3);

    // 2. Verificação de existência (has)
    assert(hashset_has(&set, &v1) == true);
    assert(hashset_has(&set, &v2) == true);
    
    int v_missing = 99;
    assert(hashset_has(&set, &v_missing) == false);

    // 3. Remoção de elementos
    assert(hashset_remove(&set, &v2) == true);
    assert(hashset_has(&set, &v2) == false);
    assert(hashset_length(&set) == 2);

    // Remover elemento inexistente
    assert(hashset_remove(&set, &v_missing) == false);

    hashset_free(&set);
    printf("[OK] Teste basico do HashSet (Add, Has, Remove) concluido.\n");
}

// Teste 2: Cópia Profunda (Copy)
void test_hashset_copy(void) {
    HashSet set1 = hashset_create(sizeof(int), int_equals, int_hash);
    int a = 1, b = 2, c = 3;

    hashset_add(&set1, &a);
    hashset_add(&set1, &b);
    hashset_add(&set1, &c);

    // Faz a cópia
    HashSet set2 = hashset_copy(&set1);
    assert(hashset_length(&set2) == 3);
    assert(hashset_has(&set2, &a) == true);
    assert(hashset_has(&set2, &b) == true);
    assert(hashset_has(&set2, &c) == true);

    // Modifica o original (adicionando d) e verifica se a cópia permanece independente
    int d = 4;
    hashset_add(&set1, &d);
    assert(hashset_length(&set1) == 4);
    assert(hashset_length(&set2) == 3);
    assert(hashset_has(&set2, &d) == false);

    hashset_free(&set1);
    hashset_free(&set2);
    printf("[OK] Teste de copia profunda (Copy) concluido.\n");
}

// Teste 3: Operações Entre Conjuntos (Union, Intersection, Difference)
void test_hashset_set_operations(void) {
    HashSet a = hashset_create(sizeof(int), int_equals, int_hash);
    HashSet b = hashset_create(sizeof(int), int_equals, int_hash);

    // A = {1, 2, 3}
    int v1 = 1, v2 = 2, v3 = 3;
    hashset_add(&a, &v1);
    hashset_add(&a, &v2);
    hashset_add(&a, &v3);

    // B = {3, 4, 5}
    int v4 = 4, v5 = 5;
    hashset_add(&b, &v3);
    hashset_add(&b, &v4);
    hashset_add(&b, &v5);

    // 1. União (Union): {1, 2, 3, 4, 5}
    HashSet u = hashset_union(&a, &b);
    assert(hashset_length(&u) == 5);
    assert(hashset_has(&u, &v1) == true);
    assert(hashset_has(&u, &v2) == true);
    assert(hashset_has(&u, &v3) == true);
    assert(hashset_has(&u, &v4) == true);
    assert(hashset_has(&u, &v5) == true);

    // 2. Intersecção (Intersection): {3}
    HashSet intersection = hashset_intersection(&a, &b);
    assert(hashset_length(&intersection) == 1);
    assert(hashset_has(&intersection, &v3) == true);
    assert(hashset_has(&intersection, &v1) == false);

    // 3. Diferença (Difference A \ B): {1, 2}
    HashSet diff = hashset_difference(&a, &b);
    assert(hashset_length(&diff) == 2);
    assert(hashset_has(&diff, &v1) == true);
    assert(hashset_has(&diff, &v2) == true);
    assert(hashset_has(&diff, &v3) == false);

    hashset_free(&a);
    hashset_free(&b);
    hashset_free(&u);
    hashset_free(&intersection);
    hashset_free(&diff);

    printf("[OK] Teste de operacoes entre conjuntos (Union, Intersection, Difference) concluido.\n");
}

// Teste 4: Iteração (Foreach)
void test_hashset_foreach(void) {
    HashSet set = hashset_create(sizeof(int), int_equals, int_hash);
    int vals[] = {10, 20, 30, 40};
    int n = sizeof(vals) / sizeof(vals[0]);

    for (int i = 0; i < n; i++) {
        hashset_add(&set, &vals[i]);
    }

    g_foreach_sum = 0;
    g_foreach_count = 0;

    hashset_foreach(&set, accumulate_foreach_func);

    assert(g_foreach_count == n);
    assert(g_foreach_sum == (10 + 20 + 30 + 40));

    hashset_free(&set);
    printf("[OK] Teste de iteracao (Foreach) concluido.\n");
}

// Teste 5: Estresse e Performance
void test_hashset_stress_and_performance(void) {
    printf("[BENCHMARK] Iniciando teste de estresse do HashSet...\n");

    HashSet set = hashset_create(sizeof(int), int_equals, int_hash);
    int total = 10000;

    clock_t start = clock();

    // Adição em massa
    for (int i = 0; i < total; i++) {
        hashset_add(&set, &i);
    }
    assert(hashset_length(&set) == total);

    // Consulta em massa (Has)
    for (int i = 0; i < total; i++) {
        assert(hashset_has(&set, &i) == true);
    }

    // Remoção da metade
    for (int i = 0; i < total / 2; i++) {
        assert(hashset_remove(&set, &i) == true);
    }
    assert(hashset_length(&set) == total / 2);

    hashset_free(&set);

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf(" -> Adicionados, consultados e removidos %d elementos.\n", total);
    printf(" -> Tempo total do teste de estresse: %.6f segundos\n", total_time);
    printf("[OK] Teste de estresse do HashSet concluido com sucesso!\n");
}

int main(void) {
    printf("--- Executando testes robustos para HashSet ---\n\n");

    test_hashset_basic();
    test_hashset_copy();
    test_hashset_set_operations();
    test_hashset_foreach();
    test_hashset_stress_and_performance();

    printf("\n--- Todos os testes do HashSet passaram com sucesso! 🎉 ---\n");
    return 0;
}