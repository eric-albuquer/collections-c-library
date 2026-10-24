#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "collections.h"

// --- STRUCT CUSTOMIZADA PARA TESTES ---
typedef struct User {
    int id;
    int age;
} User;

// Função de hash customizada para a struct User
uint64_t user_hash(const void* value) {
    if (value == NULL) return 0;
    const User* u = (const User*)value;
    // Mistura id e age usando constantes de dispersão
    uint64_t h = (uint64_t)u->id * 0xbf58476d1ce4e5b9ULL;
    h ^= (uint64_t)u->age * 0x94d049bb133111ebULL;
    return h ^ (h >> 31);
}

// Função de equals customizada para a struct User
bool user_equals(const void* a, const void* b) {
    if (a == b) return true;
    if (a == NULL || b == NULL) return false;
    const User* ua = (const User*)a;
    const User* ub = (const User*)b;
    return ua->id == ub->id && ua->age == ub->age;
}

// --- TESTES ---

// Teste 1: Operações Básicas com Chaves Inteiras e Valores Inteiros
void test_hashmap_basic_int(void) {
    // Cria um HashMap com chave int e valor int
    HashMap map = hashmap_create(sizeof(int), sizeof(int), int_equals, int_hash);
    assert(hashmap_length(&map) == 0);

    int k1 = 1, v1 = 100;
    int k2 = 2, v2 = 200;
    int k3 = 3, v3 = 300;

    // 1. Set de novos valores
    hashmap_set(&map, &k1, &v1, NULL);
    hashmap_set(&map, &k2, &v2, NULL);
    hashmap_set(&map, &k3, &v3, NULL);
    assert(hashmap_length(&map) == 3);

    // 2. Get bem-sucedido
    int out = 0;
    assert(hashmap_get(&map, &k1, &out) == true);
    assert(out == 100);

    assert(hashmap_get(&map, &k3, &out) == true);
    assert(out == 300);

    // Get com chave inexistente
    int k_inexistent = 99;
    assert(hashmap_get(&map, &k_inexistent, &out) == false);

    // 3. Atualização de valor existente e captação do old_value
    int v1_new = 150;
    int old_val = 0;
    hashmap_set(&map, &k1, &v1_new, &old_val);
    assert(old_val == 100); // Valor antigo capturado com sucesso
    assert(hashmap_length(&map) == 3); // O tamanho não deve mudar ao atualizar

    assert(hashmap_get(&map, &k1, &out) == true);
    assert(out == 150);

    // 4. Remoção de elementos
    assert(hashmap_remove(&map, &k2, &out) == true);
    assert(out == 200);
    assert(hashmap_length(&map) == 2);

    // Remover chave já removida ou inexistente
    assert(hashmap_remove(&map, &k2, &out) == false);
    assert(hashmap_get(&map, &k2, &out) == false);

    hashmap_free(&map);
    printf("[OK] Teste basico com inteiros concluido.\n");
}

// Teste 2: Testando com C-Strings (Chaves string, valores int)
void test_hashmap_string_keys(void) {
    HashMap map = hashmap_create(sizeof(char*), sizeof(int), str_equals, str_hash);

    char* k1 = "apple";
    char* k2 = "banana";
    int v1 = 10;
    int v2 = 20;

    hashmap_set(&map, &k1, &v1, NULL);
    hashmap_set(&map, &k2, &v2, NULL);
    assert(hashmap_length(&map) == 2);

    int out = 0;
    char* search_key = "apple";
    assert(hashmap_get(&map, &search_key, &out) == true);
    assert(out == 10);

    char* missing_key = "orange";
    assert(hashmap_get(&map, &missing_key, &out) == false);

    hashmap_free(&map);
    printf("[OK] Teste com chaves C-string concluido.\n");
}

// Teste 3: Testando com a struct String da biblioteca
void test_hashmap_custom_string_type(void) {
    // Usamos string_equals e string_hash (adaptados para receber ponteiro para struct String, se aplicável, ou ponteiro para os dados internos)
    // Nota: Certifique-se de que a sua lib forneça funcoes de hash/equals compatíveis com a struct String, 
    // ou passamos os ponteiros adequados conforme a API da sua lib.
    // Exemplo simulando ponteiros de String como chave:
    HashMap map = hashmap_create(sizeof(String), sizeof(int), 
        (bool(*)(const void*, const void*))string_equals, 
        (uint64_t(*)(const void*))string_hash);

    String s1 = string_create("user_one");
    String s2 = string_create("user_two");
    int v1 = 500;
    int v2 = 800;

    hashmap_set(&map, &s1, &v1, NULL);
    hashmap_set(&map, &s2, &v2, NULL);
    assert(hashmap_length(&map) == 2);

    int out = 0;
    String search = string_create("user_one");
    assert(hashmap_get(&map, &search, &out) == true);
    assert(out == 500);

    string_free(&search);
    string_free(&s1);
    string_free(&s2);
    hashmap_free(&map);

    printf("[OK] Teste com struct String da biblioteca concluido.\n");
}

// Teste 4: Testando com Struct Customizada (User)
void test_hashmap_struct_keys(void) {
    HashMap map = hashmap_create(sizeof(User), sizeof(int), user_equals, user_hash);

    User u1 = { .id = 1, .age = 25 };
    User u2 = { .id = 2, .age = 30 };
    int score1 = 95;
    int score2 = 88;

    hashmap_set(&map, &u1, &score1, NULL);
    hashmap_set(&map, &u2, &score2, NULL);
    assert(hashmap_length(&map) == 2);

    int out = 0;
    User query = { .id = 1, .age = 25 };
    assert(hashmap_get(&map, &query, &out) == true);
    assert(out == 95);

    hashmap_free(&map);
    printf("[OK] Teste com struct customizada concluido.\n");
}

// Teste 5: Estresse e Redimensionamento Automático (Rehash / Load Factor)
void test_hashmap_stress_and_rehash(void) {
    printf("[BENCHMARK] Iniciando teste de estresse e redimensionamento do HashMap...\n");

    HashMap map = hashmap_create(sizeof(int), sizeof(int), int_equals, int_hash);
    int total = 10000;

    clock_t start = clock();

    // Inserção massiva forçando redimensionamentos automáticos (rehash)
    for (int i = 0; i < total; i++) {
        int val = i * 2;
        hashmap_set(&map, &i, &val, NULL);
    }

    assert(hashmap_length(&map) == total);

    // Validação de resgate de todos os elementos inseridos
    for (int i = 0; i < total; i++) {
        int out = 0;
        assert(hashmap_get(&map, &i, &out) == true);
        assert(out == i * 2);
    }

    // Remoção em massa da metade dos elementos
    for (int i = 0; i < total / 2; i++) {
        int out = 0;
        assert(hashmap_remove(&map, &i, &out) == true);
        assert(out == i * 2);
    }

    assert(hashmap_length(&map) == total / 2);

    hashmap_free(&map);

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf(" -> Inseridas e consultadas %d chaves com sucesso.\n", total);
    printf(" -> Tempo total do teste de estresse: %.6f segundos\n", total_time);
    printf("[OK] Teste de estresse e rehash concluido com sucesso!\n");
}

int main(void) {
    printf("--- Executando testes robustos para HashMap ---\n\n");

    test_hashmap_basic_int();
    test_hashmap_string_keys();
    test_hashmap_custom_string_type();
    test_hashmap_struct_keys();
    test_hashmap_stress_and_rehash();

    printf("\n--- Todos os testes do HashMap passaram com sucesso! 🎉 ---\n");
    return 0;
}