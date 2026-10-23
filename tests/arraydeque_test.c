#include <stdio.h>
#include <assert.h>
#include "collections.h"

// Teste 1: Fluxo Básico de Inserção e Remoção (FIFO e LIFO)
void test_arraydeque_basic(void) {
    ArrayDeque deque = arraydeque_create(sizeof(int));
    assert(deque._size == 0);

    // Testando append (no final) e popleft (frente) -> Comportamento de Fila (FIFO)
    int val1 = 10, val2 = 20, val3 = 30;
    arraydeque_append(&deque, &val1);
    arraydeque_append(&deque, &val2);
    arraydeque_append(&deque, &val3);

    assert(deque._size == 3);

    int out = 0;
    assert(arraydeque_popleft(&deque, &out) == 1);
    assert(out == 10);

    assert(arraydeque_popleft(&deque, &out) == 1);
    assert(out == 20);

    assert(arraydeque_popleft(&deque, &out) == 1);
    assert(out == 30);

    assert(deque._size == 0);

    // Testando underflow (pop em deque vazia)
    assert(arraydeque_popleft(&deque, &out) == 0);
    assert(arraydeque_pop(&deque, &out) == 0);

    arraydeque_free(deque);
    printf("[OK] Teste basico concluido.\n");
}

// Teste 2: Inserção à esquerda (appendleft) e remoção à direita (pop)
void test_arraydeque_stack_and_deque_ops(void) {
    ArrayDeque deque = arraydeque_create(sizeof(int));

    int v1 = 100, v2 = 200, v3 = 300;
    
    // Inserindo na esquerda: ordem final esperada na fila -> [300, 200, 100]
    arraydeque_appendleft(&deque, &v1);
    arraydeque_appendleft(&deque, &v2);
    arraydeque_appendleft(&deque, &v3);

    assert(deque._size == 3);

    int out;
    // Pop remove do final (direita) -> deve sair v1 (100) primeiro
    assert(arraydeque_pop(&deque, &out) == 1);
    assert(out == 100);

    // Popleft remove da frente (esquerda) -> deve sair v3 (300)
    assert(arraydeque_popleft(&deque, &out) == 1);
    assert(out == 300);

    assert(deque._size == 1);

    arraydeque_free(deque);
    printf("[OK] Teste de operacoes combinadas (appendleft/pop) concluido.\n");
}

// Teste 3: Carga e Redimensionamento Dinâmico (Resize / Wraparound)
void test_arraydeque_stress_and_resize(void) {
    ArrayDeque deque = arraydeque_create(sizeof(int));
    int total_elements = 1000;

    // Inserindo uma grande quantidade de elementos para forçar resize do buffer
    for (int i = 0; i < total_elements; i++) {
        arraydeque_append(&deque, &i);
    }

    assert(deque._size == total_elements);

    // Verificando a integridade dos dados na remoção
    for (int i = 0; i < total_elements; i++) {
        int out = -1;
        int status = arraydeque_popleft(&deque, &out);
        assert(status == 1);
        assert(out == i);
    }

    assert(deque._size == 0);

    arraydeque_free(deque);
    printf("[OK] Teste de carga e redimensionamento concluido.\n");
}

// Teste 4: Verificação de Tipos Diferentes (Genéricos com void*)
void test_arraydeque_generic_types(void) {
    typedef struct {
        int x;
        int y;
    } Ponto;

    ArrayDeque deque = arraydeque_create(sizeof(Ponto));

    Ponto p1 = {10, 20};
    Ponto p2 = {30, 40};

    arraydeque_append(&deque, &p1);
    arraydeque_append(&deque, &p2);

    Ponto out;
    assert(arraydeque_popleft(&deque, &out) == 1);
    assert(out.x == 10 && out.y == 20);

    assert(arraydeque_popleft(&deque, &out) == 1);
    assert(out.x == 30 && out.y == 40);

    arraydeque_free(deque);
    printf("[OK] Teste com tipos genericos (structs) concluido.\n");
}

int main(void) {
    printf("--- Executando testes para ArrayDeque ---\n");
    
    test_arraydeque_basic();
    test_arraydeque_stack_and_deque_ops();
    test_arraydeque_stress_and_resize();
    test_arraydeque_generic_types();

    printf("--- Todos os testes passaram com sucesso! ---\n");
    return 0;
}