#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "collections.h"

// --- COMPARADORES ---

// Comparador para Min-Heap de inteiros (menor elemento tem prioridade)
int min_int_cmp(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    return arg1 - arg2;
}

// Comparador para Max-Heap de inteiros (maior elemento tem prioridade)
int max_int_cmp(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    return arg2 - arg1;
}

// Struct para teste de Fila de Prioridade com dados complexos
typedef struct {
    int prioridade;
    char nome[20];
} Tarefa;

// Comparador para Max-Heap de Tarefas (maior prioridade no topo)
int tarefa_cmp(const void* a, const void* b) {
    const Tarefa* t1 = (const Tarefa*)a;
    const Tarefa* t2 = (const Tarefa*)b;
    return t2->prioridade - t1->prioridade;
}

// --- TESTES ---

// Teste 1: Validação de Min-Heap de Inteiros
void test_heap_min_int(void) {
    Heap heap = heap_create(sizeof(int), min_int_cmp);
    assert(heap._size == 0);

    // Inserção em ordem desordenada
    int valores[] = {45, 12, 89, 3, 27, 1};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        heap_push(&heap, &valores[i]);
    }

    assert(heap._size == n);

    // Os elementos devem sair em ordem crescente: 1, 3, 12, 27, 45, 89
    int ordem_esperada[] = {1, 3, 12, 27, 45, 89};
    int out;

    for (int i = 0; i < n; i++) {
        assert(heap_pop(&heap, &out) == 1);
        assert(out == ordem_esperada[i]);
    }

    assert(heap._size == 0);

    // Teste de underflow (pop em heap vazia)
    assert(heap_pop(&heap, &out) == 0);

    heap_free(&heap);
    printf("[OK] Teste Min-Heap de inteiros concluido.\n");
}

// Teste 2: Validação de Max-Heap de Inteiros
void test_heap_max_int(void) {
    Heap heap = heap_create(sizeof(int), max_int_cmp);

    int valores[] = {10, 50, 20, 40, 30};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        heap_push(&heap, &valores[i]);
    }

    // Os elementos devem sair em ordem decrescente: 50, 40, 30, 20, 10
    int ordem_esperada[] = {50, 40, 30, 20, 10};
    int out;

    for (int i = 0; i < n; i++) {
        assert(heap_pop(&heap, &out) == 1);
        assert(out == ordem_esperada[i]);
    }

    heap_free(&heap);
    printf("[OK] Teste Max-Heap de inteiros concluido.\n");
}

// Teste 3: Teste de Carga e Redimensionamento Dinâmico
void test_heap_stress(void) {
    Heap heap = heap_create(sizeof(int), min_int_cmp);
    int total = 1000;

    // Inserindo do maior para o menor para forçar reajustes constantes (sift-up)
    for (int i = total; i > 0; i--) {
        heap_push(&heap, &i);
    }

    assert(heap._size == total);

    // Verificando se saem ordenados de 1 até 1000
    for (int i = 1; i <= total; i++) {
        int out;
        assert(heap_pop(&heap, &out) == 1);
        assert(out == i);
    }

    assert(heap._size == 0);

    heap_free(&heap);
    printf("[OK] Teste de carga e redimensionamento concluido.\n");
}

// Teste 4: Tipos Genéricos / Structs Complexas
void test_heap_structs(void) {
    Heap heap = heap_create(sizeof(Tarefa), tarefa_cmp);

    Tarefa t1 = {2, "Apresentacao"};
    Tarefa t2 = {5, "Bug Critico"};
    Tarefa t3 = {1, "Email"};
    Tarefa t4 = {5, "Outro Bug"}; // Empate na prioridade

    heap_push(&heap, &t1);
    heap_push(&heap, &t2);
    heap_push(&heap, &t3);
    heap_push(&heap, &t4);

    Tarefa out;

    // Deve sair primeiro uma das tarefas de prioridade 5
    assert(heap_pop(&heap, &out) == 1);
    assert(out.prioridade == 5);

    assert(heap_pop(&heap, &out) == 1);
    assert(out.prioridade == 5);

    // Depois a de prioridade 2
    assert(heap_pop(&heap, &out) == 1);
    assert(out.prioridade == 2);
    assert(strcmp(out.nome, "Apresentacao") == 0);

    // Por fim a de prioridade 1
    assert(heap_pop(&heap, &out) == 1);
    assert(out.prioridade == 1);

    heap_free(&heap);
    printf("[OK] Teste com structs genéricas concluido.\n");
}

int main(void) {
    printf("--- Executando testes para Heap Genérico ---\n");

    test_heap_min_int();
    test_heap_max_int();
    test_heap_stress();
    test_heap_structs();

    printf("--- Todos os testes passaram com sucesso! ---\n");
    return 0;
}