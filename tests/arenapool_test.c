#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "collections.h" // Substitua pelo arquivo onde está sua struct ArenaPool e assinaturas

// --- TESTES DE ARENAPOOL ---

// Teste 1: Criação, Inicialização e Disponibilidade Inicial
void test_arenapool_creation_and_lifecycle(void) {
    int capacidade_inicial = 1024; // 1 KB
    ArenaPool arena = arenapool_create(capacidade_inicial);
    
    // Garante que o estado inicial está perfeitamente zerado e alocado
    assert(arena._capacity == capacidade_inicial);
    assert(arena._used == 0);
    assert(arena._memory != NULL);
    assert(arenapool_available(&arena) == capacidade_inicial);

    arenapool_free(&arena);

    // Caso limite: Criação com capacidade zero ou negativa (deve ser tratada com segurança)
    ArenaPool empty = arenapool_create(0);
    assert(empty._capacity == 0);
    assert(empty._used == 0);
    assert(arenapool_available(&empty) == 0);
    arenapool_free(&empty);

    printf("[OK] Teste de criacao, ciclo de vida e disponibilidade concluido.\n");
}

// Teste 2: Alocações Sequenciais e Consumo de Espaço
void test_arenapool_sequential_allocations(void) {
    ArenaPool arena = arenapool_create(512);

    // Primeira alocação (bloco de 100 bytes)
    void* ptr1 = arenapool_malloc(&arena, 100);
    assert(ptr1 != NULL);
    assert(arena._used >= 100); // Dependendo se você implementou alinhamento, pode ser exatamente 100 ou um pouco mais
    
    int espaco_restante_1 = arenapool_available(&arena);
    assert(espaco_restante_1 <= 412);

    // Segunda alocação (bloco de 50 bytes)
    void* ptr2 = arenapool_malloc(&arena, 50);
    assert(ptr2 != NULL);
    assert(ptr2 != ptr1); // Garante que os ponteiros são distintos e contíguos
    
    // Garante que o ptr2 aponta para uma região à frente do ptr1
    assert((uint8_t*)ptr2 >= (uint8_t*)ptr1 + 100);

    arenapool_free(&arena);
    printf("[OK] Teste de alocacoes sequenciais concluido.\n");
}

// Teste 3: Estouro de Limites (Out of Memory)
void test_arenapool_out_of_memory(void) {
    ArenaPool arena = arenapool_create(64);

    // Alocação bem dentro do limite
    void* ptr1 = arenapool_malloc(&arena, 32);
    assert(ptr1 != NULL);

    // Alocação que estoura o limite restante da Arena
    void* ptr_err = arenapool_malloc(&arena, 40);
    assert(ptr_err == NULL); // Deve retornar NULL por falta de espaço de forma segura
    
    // O estado da arena não deve ser corrompido após uma falha
    assert(arena._used <= 64);
    assert(arenapool_available(&arena) >= 0);

    arenapool_free(&arena);
    printf("[OK] Teste de estouro de limite (Out of Memory) concluido.\n");
}

// Teste 4: Alinhamento de Memória (Crucial para a CPU não crashar)
void test_arenapool_memory_alignment(void) {
    ArenaPool arena = arenapool_create(128);

    // Aloca uma quantidade ímpar de bytes (força desalinhamento se não houver tratamento)
    void* ptr1 = arenapool_malloc(&arena, 3); 
    assert(ptr1 != NULL);

    // Próxima alocação. Se sua arena alinhar ponteiros (ex: múltiplos de 4 ou 8 bytes), 
    // o ptr2 deve vir em um endereço perfeitamente divisível por 4 ou 8.
    void* ptr2 = arenapool_malloc(&arena, 4);
    assert(ptr2 != NULL);

    uintptr_t endereco = (uintptr_t)ptr2;
    
    // Verifica se está alinhado em pelo menos 4 bytes (comum em arquiteturas 32/64 bits)
    // Se você optou por NÃO alinhar memória na sua implementação, pode comentar o assert abaixo.
    assert(endereco % 4 == 0); 

    arenapool_free(&arena);
    printf("[OK] Teste de comportamento de alinhamento de memoria concluido.\n");
}

// Teste 5: Isolamento de Dados e Usabilidade Concreta
void test_arenapool_data_integrity(void) {
    ArenaPool arena = arenapool_create(256);

    // Aloca espaço para uma string e um array de inteiros
    char* texto = (char*)arenapool_malloc(&arena, 12);
    int* numeros = (int*)arenapool_malloc(&arena, 4 * sizeof(int));

    assert(texto != NULL);
    assert(numeros != NULL);

    // Escreve dados nas regiões alocadas da Arena
    strcpy(texto, "Arena Test");
    numeros[0] = 42;
    numeros[1] = 84;
    numeros[2] = 126;
    numeros[3] = 168;

    // Garante que a escrita em uma região não corrompeu ou sobrescreveu a outra
    assert(strcmp(texto, "Arena Test") == 0);
    assert(numeros[0] == 42);
    assert(numeros[3] == 168);

    arenapool_free(&arena);
    printf("[OK] Teste de integridade de dados na Arena concluido.\n");
}

// Teste 6: Stress e Performance (Múltiplas Alocações Rápidas)
void test_arenapool_performance_and_stress(void) {
    printf("[BENCHMARK] Iniciando teste de estresse do ArenaPool...\n");

    clock_t start = clock();

    // Cria uma Arena grande o suficiente para milhares de pequenas alocações
    int iteracoes = 10000;
    int tamanho_bloco = 8;
    ArenaPool arena = arenapool_create(iteracoes * tamanho_bloco * 2);

    // Realiza milhares de alocações consecutivas (O(1) para cada alocação)
    for (int i = 0; i < iteracoes; i++) {
        void* ptr = arenapool_malloc(&arena, tamanho_bloco);
        assert(ptr != NULL);
    }

    assert(arena._used >= (iteracoes * tamanho_bloco));
    
    // O reset/free da arena inteira acontece de uma vez só (O(1))
    arenapool_free(&arena);

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf(" -> Iteracoes: %d alocações contíguas em bloco\n", iteracoes);
    printf(" -> Tempo total do teste: %.6f segundos\n", total_time);
    printf("[OK] Teste de performance e estresse do ArenaPool concluido com sucesso!\n");
}

// --- FUNÇÃO MAIN ---
int main(void) {
    printf("--- Executando testes robustos para ArenaPool ---\n\n");

    test_arenapool_creation_and_lifecycle();
    test_arenapool_sequential_allocations();
    test_arenapool_out_of_memory();
    test_arenapool_memory_alignment();
    test_arenapool_data_integrity();
    test_arenapool_performance_and_stress();

    printf("\n--- Todos os testes do ArenaPool passaram com sucesso! 🚀 ---\n");
    return 0;
}
