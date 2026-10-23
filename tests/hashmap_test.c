#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "collections.h"

// --- FUNÇÕES AUXILIARES DE HASH E EQUALS ---

unsigned int int_hash(const void* key) {
    unsigned int x = *(const unsigned int*)key;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

int int_equals(const void* a, const void* b) {
    return *(const int*)a == *(const int*)b;
}

unsigned int bad_hash_collision(const void* key) {
    (void)key;
    return 42; // Força colisões
}

// --- TESTES ---

// Teste 1: Validação do Retorno de Substituição vs Inserção
void test_hashmap_set_return_values(void) {
    HashMap map = hashmap_create(sizeof(int), sizeof(int), int_equals, int_hash);

    int k1 = 10, v1 = 100;
    int v1_novo = 150;
    int old_value = 0;

    // 1. PRIMEIRA INSERÇÃO: Deve indicar que a chave É NOVA (retorno diferente de 1, ex: 0)
    int status_insert = hashmap_set(&map, &k1, &v1, NULL);
    assert(status_insert != 1); 
    assert(map._size == 1);

    // 2. SUBSTITUIÇÃO (UPDATE): Deve retornar 1 indicando substituição
    int status_update = hashmap_set(&map, &k1, &v1_novo, &old_value);
    assert(status_update == 1); // <--- Valida a sua regra de retorno
    assert(old_value == 100);    // Garante que recuperou o valor antigo correto
    assert(map._size == 1);     // O tamanho da tabela não pode aumentar no update

    // Confirma se o valor foi atualizado na busca
    int out = 0;
    assert(hashmap_get(&map, &k1, &out) == 1);
    assert(out == 150);

    hashmap_free(map);
    printf("[OK] Teste dos retornos do hashmap_set (insert vs update) concluido.\n");
}

// Teste 2: CRUD Básico e Integridade dos Dados
void test_hashmap_crud(void) {
    HashMap map = hashmap_create(sizeof(int), sizeof(int), int_equals, int_hash);

    int keys[] = {1, 2, 3};
    int values[] = {10, 20, 30};

    // Inserindo chaves novas
    for (int i = 0; i < 3; i++) {
        assert(hashmap_set(&map, &keys[i], &values[i], NULL) != 1);
    }
    assert(map._size == 3);

    // Atualizando todas as chaves existentes (todas devem retornar 1)
    for (int i = 0; i < 3; i++) {
        int novo_val = values[i] * 10;
        int val_antigo = 0;
        assert(hashmap_set(&map, &keys[i], &novo_val, &val_antigo) == 1);
        assert(val_antigo == values[i]);
    }
    assert(map._size == 3);

    // Leitura dos novos valores
    for (int i = 0; i < 3; i++) {
        int out = 0;
        assert(hashmap_get(&map, &keys[i], &out) == 1);
        assert(out == values[i] * 10);
    }

    // Remoção
    int out_del = 0;
    assert(hashmap_remove(&map, &keys[1], &out_del) == 1);
    assert(out_del == 200);
    assert(map._size == 2);

    // Garantir que chave removida não é mais encontrada
    assert(hashmap_get(&map, &keys[1], &out_del) == 0);

    hashmap_free(map);
    printf("[OK] Teste CRUD basico concluido.\n");
}

// Teste 3: Comportamento sob Colisão com Substituição
void test_hashmap_collision_update(void) {
    HashMap map = hashmap_create(sizeof(int), sizeof(int), int_equals, bad_hash_collision);

    int k1 = 5, v1 = 50;
    int k2 = 15, v2 = 150; // Mesma posição de hash de k1

    assert(hashmap_set(&map, &k1, &v1, NULL) != 1);
    assert(hashmap_set(&map, &k2, &v2, NULL) != 1);
    assert(map._size == 2);

    // Atualiza k2 mesmo estando em uma cadeia de colisão
    int v2_novo = 999;
    int old_val = 0;
    assert(hashmap_set(&map, &k2, &v2_novo, &old_val) == 1);
    assert(old_val == 150);

    int out = 0;
    assert(hashmap_get(&map, &k2, &out) == 1);
    assert(out == 999);

    hashmap_free(map);
    printf("[OK] Teste de update em colisoes concluido.\n");
}

// Teste 4: Carga e Redimensionamento (Rehash)
void test_hashmap_stress(void) {
    HashMap map = hashmap_create(sizeof(int), sizeof(int), int_equals, int_hash);
    int total = 500;

    // 1. Inserção massiva de novos elementos
    for (int i = 0; i < total; i++) {
        int val = i * 2;
        assert(hashmap_set(&map, &i, &val, NULL) != 1);
    }
    assert(map._size == total);

    // 2. Atualização massiva de todos os elementos
    for (int i = 0; i < total; i++) {
        int val_novo = i * 3;
        int val_antigo = 0;
        assert(hashmap_set(&map, &i, &val_novo, &val_antigo) == 1);
        assert(val_antigo == i * 2);
    }
    assert(map._size == total);

    // 3. Validação final da leitura
    for (int i = 0; i < total; i++) {
        int out = 0;
        assert(hashmap_get(&map, &i, &out) == 1);
        assert(out == i * 3);
    }

    hashmap_free(map);
    printf("[OK] Teste de carga e rehash concluido.\n");
}

int main(void) {
    printf("--- Executando testes para HashMap ---\n");

    test_hashmap_set_return_values();
    test_hashmap_crud();
    test_hashmap_collision_update();
    test_hashmap_stress();

    printf("--- Todos os testes passaram com sucesso! ---\n");
    return 0;
}