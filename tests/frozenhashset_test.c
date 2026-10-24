#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "collections.h"

// --- VARIÁVEIS GLOBAIS AUXILIARES PARA TESTES ---
static int g_frozen_foreach_sum = 0;
static int g_frozen_foreach_count = 0;

void accumulate_frozen_foreach_func(const void* element) {
    g_frozen_foreach_sum += *(const int*)element;
    g_frozen_foreach_count++;
}

// --- TESTES ---

// Teste 1: Criação, Checksum, Length e Has
void test_frozenhashset_basic(void) {
    // 1. Cria um HashSet comum mutável
    HashSet set = hashset_create(sizeof(int), int_equals, int_hash);
    int v1 = 10, v2 = 20, v3 = 30;

    hashset_add(&set, &v1);
    hashset_add(&set, &v2);
    hashset_add(&set, &v3);

    // 2. Congela o HashSet transformando-o em FrozenHashSet
    FrozenHashSet fset = frozenhashset_create(set);

    assert(frozenhashset_length(&fset) == 3);

    // 3. Verifica existência de elementos (has)
    assert(frozenhashset_has(&fset, &v1) == true);
    assert(frozenhashset_has(&fset, &v2) == true);
    assert(frozenhashset_has(&fset, &v3) == true);
    
    int v_missing = 99;
    assert(frozenhashset_has(&fset, &v_missing) == false);

    // 4. Verifica o checksum (hash do frozen set)
    uint64_t h1 = frozenhashset_hash(&fset);
    assert(h1 != 0);

    frozenhashset_free(&fset);
    printf("[OK] Teste basico do FrozenHashSet (Create, Checksum, Has) concluido.\n");
}

// Teste 2: Igualdade entre FrozenHashSets (Usando o Checksum Otimizado)
void test_frozenhashset_equals(void) {
    // Conjunto A = {1, 2, 3}
    HashSet setA = hashset_create(sizeof(int), int_equals, int_hash);
    int x1 = 1, x2 = 2, x3 = 3;
    hashset_add(&setA, &x1);
    hashset_add(&setA, &x2);
    hashset_add(&setA, &x3);
    FrozenHashSet fsetA = frozenhashset_create(setA);

    // Conjunto B = {3, 1, 2} (mesmos elementos, ordem diferente no hash map)
    HashSet setB = hashset_create(sizeof(int), int_equals, int_hash);
    hashset_add(&setB, &x3);
    hashset_add(&setB, &x1);
    hashset_add(&setB, &x2);
    FrozenHashSet fsetB = frozenhashset_create(setB);

    // Conjunto C = {1, 2, 4} (diferente)
    HashSet setC = hashset_create(sizeof(int), int_equals, int_hash);
    int x4 = 4;
    hashset_add(&setC, &x1);
    hashset_add(&setC, &x2);
    hashset_add(&setC, &x4);
    FrozenHashSet fsetdataC = frozenhashset_create(setC);

    // Validações de igualdade
    assert(frozenhashset_equals(&fsetA, &fsetB) == true);
    assert(frozenhashset_equals(&fsetA, &fsetdataC) == false);

    // Verifica se o checksum de conjuntos idênticos é estritamente igual
    assert(frozenhashset_hash(&fsetA) == frozenhashset_hash(&fsetB));
    assert(frozenhashset_hash(&fsetA) != frozenhashset_hash(&fsetdataC));

    frozenhashset_free(&fsetA);
    frozenhashset_free(&fsetB);
    frozenhashset_free(&fsetdataC);

    printf("[OK] Teste de igualdade e checksum do FrozenHashSet concluido.\n");
}

// Teste 3: Operações Entre FrozenHashSets (Union, Intersection, Difference)
void test_frozenhashset_set_operations(void) {
    // A = {1, 2, 3}
    HashSet sa = hashset_create(sizeof(int), int_equals, int_hash);
    int v1 = 1, v2 = 2, v3 = 3;
    hashset_add(&sa, &v1);
    hashset_add(&sa, &v2);
    hashset_add(&sa, &v3);
    FrozenHashSet fa = frozenhashset_create(sa);

    // B = {3, 4, 5}
    HashSet sb = hashset_create(sizeof(int), int_equals, int_hash);
    int v4 = 4, v5 = 5;
    hashset_add(&sb, &v3);
    hashset_add(&sb, &v4);
    hashset_add(&sb, &v5);
    FrozenHashSet fb = frozenhashset_create(sb);

    // 1. União (Union): {1, 2, 3, 4, 5}
    FrozenHashSet fu = frozenhashset_union(&fa, &fb);
    assert(frozenhashset_length(&fu) == 5);
    assert(frozenhashset_has(&fu, &v1) == true);
    assert(frozenhashset_has(&fu, &v3) == true);
    assert(frozenhashset_has(&fu, &v5) == true);

    // 2. Intersecção (Intersection): {3}
    FrozenHashSet fintersect = frozenhashset_intersection(&fa, &fb);
    assert(frozenhashset_length(&fintersect) == 1);
    assert(frozenhashset_has(&fintersect, &v3) == true);
    assert(frozenhashset_has(&fintersect, &v1) == false);

    // 3. Diferença (Difference A \ B): {1, 2}
    FrozenHashSet fdiff = frozenhashset_difference(&fa, &fb);
    assert(frozenhashset_length(&fdiff) == 2);
    assert(frozenhashset_has(&fdiff, &v1) == true);
    assert(frozenhashset_has(&fdiff, &v2) == true);
    assert(frozenhashset_has(&fdiff, &v3) == false);

    frozenhashset_free(&fa);
    frozenhashset_free(&fb);
    frozenhashset_free(&fu);
    frozenhashset_free(&fintersect);
    frozenhashset_free(&fdiff);

    printf("[OK] Teste de operacoes entre FrozenHashSets concluido.\n");
}

// Teste 4: Iteração (Foreach)
void test_frozenhashset_foreach(void) {
    HashSet set = hashset_create(sizeof(int), int_equals, int_hash);
    int vals[] = {5, 15, 25};
    int n = sizeof(vals) / sizeof(vals[0]);

    for (int i = 0; i < n; i++) {
        hashset_add(&set, &vals[i]);
    }

    FrozenHashSet fset = frozenhashset_create(set);

    g_frozen_foreach_sum = 0;
    g_frozen_foreach_count = 0;

    frozenhashset_foreach(&fset, accumulate_frozen_foreach_func);

    assert(g_frozen_foreach_count == n);
    assert(g_frozen_foreach_sum == (5 + 15 + 25));

    frozenhashset_free(&fset);
    printf("[OK] Teste de iteracao (Foreach) no FrozenHashSet concluido.\n");
}

// Teste 5: Estresse e Performance do Checksum e Criação
void test_frozenhashset_stress_and_performance(void) {
    printf("[BENCHMARK] Iniciando teste de estresse do FrozenHashSet...\n");

    HashSet set = hashset_create(sizeof(int), int_equals, int_hash);
    int total = 5000;

    for (int i = 0; i < total; i++) {
        hashset_add(&set, &i);
    }

    clock_t start = clock();

    // Cria o frozen set (calculando o checksum em lote)
    FrozenHashSet fset = frozenhashset_create(set);
    assert(frozenhashset_length(&fset) == total);

    // Testa consultas e integridade do checksum cacheado
    uint64_t checksum_cached = frozenhashset_hash(&fset);
    assert(checksum_cached != 0);

    for (int i = 0; i < total; i++) {
        assert(frozenhashset_has(&fset, &i) == true);
    }

    // Testa igualdade rápida baseada no checksum pré-calculado
    FrozenHashSet fset_copy = frozenhashset_create(hashset_copy(&fset._set));
    assert(frozenhashset_equals(&fset, &fset_copy) == true);

    frozenhashset_free(&fset);
    frozenhashset_free(&fset_copy);

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf(" -> Congelados e validados %d elementos com checksum O(1).\n", total);
    printf(" -> Tempo total do teste de estresse: %.6f segundos\n", total_time);
    printf("[OK] Teste de estresse do FrozenHashSet concluido com sucesso!\n");
}

int main(void) {
    printf("--- Executando testes robustos para FrozenHashSet ---\n\n");

    test_frozenhashset_basic();
    test_frozenhashset_equals();
    test_frozenhashset_set_operations();
    test_frozenhashset_foreach();
    test_frozenhashset_stress_and_performance();

    printf("\n--- Todos os testes do FrozenHashSet passaram com sucesso! 🎉 ---\n");
    return 0;
}