#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "collections.h"

// --- FUNÇÕES AUXILIARES DE HASH E EQUALS ---

// Hash ruim para forçar colisões
uint64_t bad_hash_collision(const void* key) {
    return 100;
}

// --- TESTES ---

// Teste 1: Operações Básicas (Adicionar, Verificar, Unicidade e Remover)
void test_hashset_basic(void) {
    HashSet set = hashset_create(sizeof(int), int_equals, int_hash);
    assert(set._size == 0);

    int v1 = 10, v2 = 20, v3 = 30;

    // 1. Inserção
    hashset_add(&set, &v1);
    hashset_add(&set, &v2);
    hashset_add(&set, &v3);
    assert(set._size == 3);

    // 2. Verificação de Existência (has)
    assert(hashset_has(&set, &v1) == 1);
    assert(hashset_has(&set, &v2) == 1);
    assert(hashset_has(&set, &v3) == 1);

    int v_absent = 99;
    assert(hashset_has(&set, &v_absent) == 0);

    // 3. Unicidade: Adicionar elemento duplicado não deve aumentar o tamanho
    hashset_add(&set, &v1);
    assert(set._size == 3);

    // 4. Remoção
    assert(hashset_remove(&set, &v2) == 1);
    assert(set._size == 2);
    assert(hashset_has(&set, &v2) == 0);

    // Remover elemento já removido ou inexistente deve retornar 0
    assert(hashset_remove(&set, &v2) == 0);
    assert(hashset_remove(&set, &v_absent) == 0);

    hashset_free(&set);
    printf("[OK] Teste basico concluido.\n");
}

// Teste 2: Tratamento de Colisões
void test_hashset_collisions(void) {
    HashSet set = hashset_create(sizeof(int), int_equals, bad_hash_collision);

    int values[] = {1, 2, 3, 4, 5};
    int count = 5;

    for (int i = 0; i < count; i++) {
        hashset_add(&set, &values[i]);
    }

    assert(set._size == count);

    // Todos devem ser encontrados mesmo com colisão na mesma posição inicial
    for (int i = 0; i < count; i++) {
        assert(hashset_has(&set, &values[i]) == 1);
    }

    // Remove elemento do meio da cadeia de colisão
    assert(hashset_remove(&set, &values[2]) == 1); // Remove '3'
    assert(set._size == count - 1);

    // Garante que o elemento posterior continua acessível
    assert(hashset_has(&set, &values[4]) == 1);

    hashset_free(&set);
    printf("[OK] Teste de colisoes concluido.\n");
}

// Teste 3: Carga e Redimensionamento Dinâmico (Rehash)
void test_hashset_stress(void) {
    HashSet set = hashset_create(sizeof(int), int_equals, int_hash);
    int total = 1000;

    // Inserção massiva
    for (int i = 0; i < total; i++) {
        hashset_add(&set, &i);
    }

    assert(set._size == total);

    // Tentar re-inserir todos (tamanho deve permanecer 'total')
    for (int i = 0; i < total; i++) {
        hashset_add(&set, &i);
    }

    assert(set._size == total);

    // Leitura após rehash
    for (int i = 0; i < total; i++) {
        assert(hashset_has(&set, &i) == 1);
    }

    // Remoção de metade
    for (int i = 0; i < total / 2; i++) {
        assert(hashset_remove(&set, &i) == 1);
    }

    assert(set._size == total / 2);

    // Validação do estado final
    for (int i = 0; i < total; i++) {
        if (i < total / 2) {
            assert(hashset_has(&set, &i) == 0);
        } else {
            assert(hashset_has(&set, &i) == 1);
        }
    }

    hashset_free(&set);
    printf("[OK] Teste de carga e rehash concluido.\n");
}

// Teste 4: Uso com Tipos Ponteiro/String (`char*`)
void test_hashset_strings(void) {
    HashSet set = hashset_create(sizeof(char*), str_equals, str_hash);

    const char* s1 = "C";
    const char* s2 = "C++";
    const char* s3 = "Rust";

    hashset_add(&set, &s1);
    hashset_add(&set, &s2);
    hashset_add(&set, &s3);

    assert(set._size == 3);

    const char* query = "C++";
    assert(hashset_has(&set, &query) == 1);

    const char* query_absent = "Java";
    assert(hashset_has(&set, &query_absent) == 0);

    hashset_free(&set);
    printf("[OK] Teste com strings concluido.\n");
}

int main(void) {
    printf("--- Executando testes para HashSet ---\n");

    test_hashset_basic();
    test_hashset_collisions();
    test_hashset_stress();
    test_hashset_strings();

    printf("--- Todos os testes passaram com sucesso! ---\n");
    return 0;
}