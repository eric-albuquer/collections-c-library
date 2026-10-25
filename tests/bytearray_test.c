#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "collections.h" // Substitua pelo arquivo onde está sua struct e assinaturas

// --- TESTES DE BYTEARRAY ---

// Teste 1: Inicialização, Capacidade Inicial e Casos Limite
void test_bytearray_creation_and_lifecycle(void) {
    // Criação normal
    ByteArray ba = bytearray_create(10);
    assert(ba._capacity == 10);
    assert(ba._size == 0);
    assert(ba._cursor == 0);
    assert(ba._data != NULL);
    
    // Teste do Clear (deve zerar tamanho e cursor, mantendo capacidade)
    ba._size = 5;
    ba._cursor = 3;
    bytearray_clear(&ba);
    assert(ba._size == 0);
    assert(ba._cursor == 0);
    assert(ba._capacity == 10); // Memória continua lá

    bytearray_free(&ba);

    // Caso limite: Criação com capacidade zero (deve ser tratada com segurança)
    ByteArray empty = bytearray_create(0);
    assert(empty._size == 0);
    assert(empty._cursor == 0);
    bytearray_free(&empty);

    printf("[OK] Teste de criacao, ciclo de vida e edge cases concluido.\n");
}

// Teste 2: Reserva Estratégica de Memória (Capacidade Absoluta)
void test_bytearray_reserve(void) {
    ByteArray ba = bytearray_create(16);
    
    // Reservar tamanho maior deve realocar
    bytearray_reserve(&ba, 64);
    assert(ba._capacity == 64);
    assert(ba._size == 0);

    // Reservar tamanho menor ou igual NÃO deve fazer nada (evita reallocs atoa)
    bytearray_reserve(&ba, 32);
    assert(ba._capacity == 64); 

    bytearray_free(&ba);
    printf("[OK] Teste de reserva (capacity) concluido.\n");
}

// Teste 3: Fluxo de Escrita e Crescimento Automático
void test_bytearray_write_and_expansion(void) {
    // Cria um buffer pequeno de 4 bytes
    ByteArray ba = bytearray_create(4);
    
    uint8_t dados1[4] = {0xAA, 0xBB, 0xCC, 0xDD};
    bytearray_write_bytes(&ba, dados1, 4);
    assert(ba._size == 4);
    assert(ba._cursor == 4);
    assert(ba._capacity == 4);

    // Escrever mais dados deve disparar a expansão automática (via reserve interno)
    uint8_t dados2[2] = {0xEE, 0xFF};
    bytearray_write_bytes(&ba, dados2, 2);
    assert(ba._size == 6);
    assert(ba._cursor == 6);
    assert(ba._capacity > 4); // Garante que expandiu

    // Validação de dados escritos diretamente na memória interna
    assert(ba._data[0] == 0xAA);
    assert(ba._data[4] == 0xEE);
    assert(ba._data[5] == 0xFF);

    bytearray_free(&ba);
    printf("[OK] Teste de escrita e expansao automatica concluido.\n");
}

// Teste 4: Fluxo de Leitura Sequencial e Casos de Erro (Overflow)
void test_bytearray_read_and_bounds(void) {
    ByteArray ba = bytearray_create(8);
    uint8_t input[5] = {10, 20, 30, 40, 50};
    bytearray_write_bytes(&ba, input, 5);

    // Reseta o cursor para o início para podermos ler sequencialmente
    bytearray_seek(&ba, 0);
    assert(ba._cursor == 0);

    // Leitura parcial (primeiros 2 bytes)
    uint8_t out1[2] = {0};
    int bytes_lidos1 = bytearray_read_bytes(&ba, out1, 2);
    assert(bytes_lidos1 == 2);
    assert(out1[0] == 10);
    assert(out1[1] == 20);
    assert(ba._cursor == 2); // Cursor avançou sequencialmente

    // Leitura do restante (próximos 3 bytes)
    uint8_t out2[3] = {0};
    int bytes_lidos2 = bytearray_read_bytes(&ba, out2, 3);
    assert(bytes_lidos2 == 3);
    assert(out2[0] == 30);
    assert(out2[2] == 50);
    assert(ba._cursor == 5);
    assert(ba._cursor == ba._size); // Chegou ao fim lógico do dado

    // Cenário de erro: Tentar ler além do tamanho gravado (Overflow)
    uint8_t out_err[1];
    int bytes_lidos_err = bytearray_read_bytes(&ba, out_err, 1);
    // Dependendo do seu design, deve retornar 0 bytes lidos ou -1 para erro
    assert(bytes_lidos_err <= 0); 
    assert(ba._cursor == 5); // O cursor não deve se mover em caso de falha

    bytearray_free(&ba);
    printf("[OK] Teste de leitura sequencial e limites de segurança concluido.\n");
}

// Teste 5: Navegação de Cursor (Seek Absoluto) e Sobrescrita
void test_bytearray_seek_and_overwrite(void) {
    ByteArray ba = bytearray_create(10);
    uint8_t dados[4] = {0x01, 0x02, 0x03, 0x04};
    bytearray_write_bytes(&ba, dados, 4); // tamanho=4, cursor=4

    // Seek para trás (posição inicial)
    bool s1 = bytearray_seek(&ba, 0);
    assert(s1 == true);
    assert(ba._cursor == 0);

    // Sobrescreve apenas os 2 primeiros bytes
    uint8_t novos_dados[2] = {0x99, 0x88};
    bytearray_write_bytes(&ba, novos_dados, 2); // tamanho continua 4, cursor vira 2

    assert(ba._size == 4); 
    assert(ba._cursor == 2);
    assert(ba._data[0] == 0x99);
    assert(ba._data[1] == 0x88);
    assert(ba._data[2] == 0x03); // Manteve o dado antigo intacto

    // Caso de Erro do Seek: Fora dos limites do tamanho atual
    bool s_err = bytearray_seek(&ba, 10);
    assert(s_err == false); // Não pode pular para o meio do nada
    assert(ba._cursor == 2); // Mantém a posição segura anterior

    bytearray_free(&ba);
    printf("[OK] Teste de movimentacao de cursor (seek) concluido.\n");
}

// Teste 6: Stress e Performance (Benchmark de realocação)
void test_bytearray_performance_and_stress(void) {
    printf("[BENCHMARK] Iniciando teste de estresse do ByteArray...\n");

    clock_t start = clock();

    // Começa com capacidade minúscula (1 byte) para forçar múltiplos reallocs ao longo do loop
    ByteArray ba = bytearray_create(1);
    int iteracoes = 50000;
    uint8_t byte_ficticio = 0xAA;

    for (int i = 0; i < iteracoes; i++) {
        // Gravando 1 byte por vez para testar a eficiência da estratégia de expansão
        bytearray_write_bytes(&ba, &byte_ficticio, 1);
    }

    assert(ba._size == iteracoes);
    assert(ba._cursor == iteracoes);
    assert(ba._capacity >= (size_t)iteracoes);

    // Teste de leitura em massa
    bytearray_seek(&ba, 0);
    uint8_t* buffer_saida = (uint8_t*)malloc(iteracoes);
    assert(buffer_saida != NULL);

    int lidos = bytearray_read_bytes(&ba, buffer_saida, iteracoes);
    assert(lidos == iteracoes);
    assert(buffer_saida[iteracoes - 1] == 0xAA);

    free(buffer_saida);
    bytearray_free(&ba);

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf(" -> Iteracoes: %d escritas sequenciais de 1 byte\n", iteracoes);
    printf(" -> Tempo total do teste: %.6f segundos\n", total_time);
    printf("[OK] Teste de performance e estresse concluido com sucesso!\n");
}

// --- FUNÇÃO MAIN ---
int main(void) {
    printf("--- Executando testes robustos para ByteArray ---\n\n");

    test_bytearray_creation_and_lifecycle();
    test_bytearray_reserve();
    test_bytearray_write_and_expansion();
    test_bytearray_read_and_bounds();
    test_bytearray_seek_and_overwrite();
    test_bytearray_performance_and_stress();

    printf("\n--- Todos os testes do ByteArray passaram com sucesso! 🚀 ---\n");
    return 0;
}
