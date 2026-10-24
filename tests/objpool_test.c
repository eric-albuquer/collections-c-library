#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "collections.h"

// Struct auxiliar para testes do ObjectPool
typedef struct Point {
    int x;
    int y;
} Point;

// --- TESTES ---

// Teste 1: Criação, Alocação Básica e Liberação
void test_objpool_basic(void) {
    int capacity = 5;
    ObjectPool pool = objpool_create(capacity, sizeof(Point));
    assert(pool._capacity == capacity);
    assert(pool._obj_size >= sizeof(Point)); // Pode incluir padding de alinhamento
    assert(pool._used == 0);

    // Aloca 3 objetos
    Point* p1 = (Point*)objpool_malloc(&pool);
    Point* p2 = (Point*)objpool_malloc(&pool);
    Point* p3 = (Point*)objpool_malloc(&pool);

    assert(p1 != NULL);
    assert(p2 != NULL);
    assert(p3 != NULL);
    assert(pool._used == 3);

    // Atribui valores e testa integridade
    p1->x = 10; p1->y = 20;
    p2->x = 30; p2->y = 40;
    p3->x = 50; p3->y = 60;

    assert(p1->x == 10 && p1->y == 20);
    assert(p2->x == 30 && p2->y == 40);
    assert(p3->x == 50 && p3->y == 60);

    // Libera um objeto
    objpool_free_obj(&pool, p2);
    assert(pool._used == 2);

    objpool_free(&pool);
    printf("[OK] Teste basico do ObjectPool (Create, Malloc, Free) concluido.\n");
}

// Teste 2: Reutilização de Blocos (Free List / _head)
void test_objpool_reuse(void) {
    ObjectPool pool = objpool_create(3, sizeof(int));

    int* a = (int*)objpool_malloc(&pool);
    int* b = (int*)objpool_malloc(&pool);
    int* c = (int*)objpool_malloc(&pool);

    *a = 100;
    *b = 200;
    *c = 300;

    // Libera o meio (b)
    objpool_free_obj(&pool, b);
    assert(pool._used == 2);

    // Aloca novamente: deve reutilizar exatamente o slot que b liberou (LIFO via _head)
    int* d = (int*)objpool_malloc(&pool);
    assert(d == b); 
    *d = 999;

    assert(*a == 100);
    assert(*c == 300);
    assert(*d == 999);
    assert(pool._used == 3);

    objpool_free(&pool);
    printf("[OK] Teste de reutilizacao de blocos do ObjectPool concluido.\n");
}

// Teste 3: Limite de Capacidade e Edge Cases
void test_objpool_capacity_limits(void) {
    ObjectPool pool = objpool_create(2, sizeof(int));

    int* p1 = (int*)objpool_malloc(&pool);
    int* p2 = (int*)objpool_malloc(&pool);
    assert(p1 != NULL);
    assert(p2 != NULL);
    assert(pool._used == 2);

    // Tentar alocar quando o pool está cheio deve retornar NULL
    int* p_overflow = (int*)objpool_malloc(&pool);
    assert(p_overflow == NULL);

    // Liberar um e tentar alocar novamente deve funcionar
    objpool_free_obj(&pool, p1);
    assert(pool._used == 1);

    int* p3 = (int*)objpool_malloc(&pool);
    assert(p3 != NULL);
    assert(p3 == p1); // Reutilizou p1
    assert(pool._used == 2);

    objpool_free(&pool);
    printf("[OK] Teste de limites de capacidade do ObjectPool concluido.\n");
}

// Teste 4: Estresse e Performance (Benchmark)
void test_objpool_stress_and_performance(void) {
    printf("[BENCHMARK] Iniciando teste de estresse do ObjectPool...\n");

    int capacity = 50000;
    ObjectPool pool = objpool_create(capacity, sizeof(int));

    clock_t start = clock();

    // 1. Aloca todos os objetos do pool
    int** ptrs = (int**)malloc(capacity * sizeof(int*));
    for (int i = 0; i < capacity; i++) {
        ptrs[i] = (int*)objpool_malloc(&pool);
        assert(ptrs[i] != NULL);
        *ptrs[i] = i;
    }
    assert(pool._used == capacity);

    // 2. Libera todos os objetos
    for (int i = 0; i < capacity; i++) {
        objpool_free_obj(&pool, ptrs[i]);
    }
    assert(pool._used == 0);

    // 3. Realoca tudo novamente para testar a robustez da free list cheia
    for (int i = 0; i < capacity; i++) {
        ptrs[i] = (int*)objpool_malloc(&pool);
        assert(ptrs[i] != NULL);
    }
    assert(pool._used == capacity);

    free(ptrs);
    objpool_free(&pool);

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf(" -> Realizadas %d alocacoes e liberacoes em lote.\n", capacity * 2);
    printf(" -> Tempo total do teste de estresse: %.6f segundos\n", total_time);
    printf("[OK] Teste de estresse do ObjectPool concluido com sucesso!\n");
}

int main(void) {
    printf("--- Executando testes robustos para ObjectPool ---\n\n");

    test_objpool_basic();
    test_objpool_reuse();
    test_objpool_capacity_limits();
    test_objpool_stress_and_performance();

    printf("\n--- Todos os testes do ObjectPool passaram com sucesso! 🎉 ---\n");
    return 0;
}