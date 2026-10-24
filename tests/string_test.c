#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "collections.h"

// --- TESTES ---

// Teste 1: Criação, Getters e Casos Limite (Edge Cases)
void test_string_creation_and_getters(void) {
    // String normal
    String s = string_create("Hello");
    assert(string_length(&s) == 5);
    assert(strcmp(string_get_str(&s), "Hello") == 0);
    assert(s._capacity >= 5);
    string_free(&s);

    // String vazia
    String empty = string_create("");
    assert(string_length(&empty) == 0);
    assert(strcmp(string_get_str(&empty), "") == 0);
    string_free(&empty);

    printf("[OK] Teste de criacao, getters e edge cases concluido.\n");
}

// Teste 2: Igualdade e Comparação Lexicográfica
void test_string_equality_and_compare(void) {
    String s1 = string_create("Hello");
    String s2 = string_create("Hello");
    String s3 = string_create("World");
    String s4 = string_create("Hell");

    // 1. Igualdade
    assert(string_equals(&s1, &s2) == true);
    assert(string_equals(&s1, &s3) == false);
    
    // 2. Comparação
    assert(string_compare(&s1, &s3) < 0);  // "Hello" < "World"
    assert(string_compare(&s3, &s1) > 0);  // "World" > "Hello"
    assert(string_compare(&s1, &s2) == 0); // "Hello" == "Hello"
    assert(string_compare(&s1, &s4) > 0);  // "Hello" > "Hell" (prefixo)

    string_free(&s1);
    string_free(&s2);
    string_free(&s3);
    string_free(&s4);
    
    printf("[OK] Teste de igualdade e comparacao concluido.\n");
}

// Teste 3: Manipulação (Append de Char, String e Invalidação de Hash)
void test_string_manipulation(void) {
    String s1 = string_create("Hello");
    
    // Garante que o hash inicial funciona e ativa o cache
    uint64_t h_initial = string_hash(&s1);
    assert(s1._hash_computed == true);

    // 1. Append de caractere deve invalidar o hash
    string_append_char(&s1, '!');
    assert(s1._hash_computed == false);
    assert(string_length(&s1) == 6);
    assert(strcmp(string_get_str(&s1), "Hello!") == 0);

    // 2. Append de outra String
    String s2 = string_create(" C");
    string_extend(&s1, &s2);
    assert(s1._hash_computed == false);
    assert(string_length(&s1) == 8);
    assert(strcmp(string_get_str(&s1), "Hello! C") == 0);

    string_free(&s1);
    string_free(&s2);
    
    printf("[OK] Teste de manipulacao e invalidação de cache concluido.\n");
}

// Teste 4: Cópia e Limpeza (Copy e Clear)
void test_string_copy_and_clear(void) {
    String s1 = string_create("Testando");
    
    // 1. Cópia profunda (deep copy)
    String s_copy = string_copy(&s1);
    assert(string_equals(&s1, &s_copy) == true);
    
    // Modificar a cópia não deve afetar a original
    string_append_char(&s_copy, 's');
    assert(string_equals(&s1, &s_copy) == false);

    // 2. Clear
    string_clear(&s1);
    assert(string_length(&s1) == 0);
    assert(strcmp(string_get_str(&s1), "") == 0);
    assert(s1._hash_computed == false);

    string_free(&s1);
    string_free(&s_copy);
    
    printf("[OK] Teste de copia e limpeza concluido.\n");
}

// Teste 5: Cálculo, Cache e Integridade de Hash
void test_string_hash(void) {
    String s = string_create("Collections");
    
    uint64_t h1 = string_hash(&s);
    assert(s._hash_computed == true);
    
    // Leitura subsequente deve ser rápida e igual (via cache)
    uint64_t h2 = string_hash(&s);
    assert(h1 == h2);
    assert(h1 != 0);

    // Modificar a string recalcula o hash corretamente
    string_append_char(&s, '!');
    assert(s._hash_computed == false);
    uint64_t h3 = string_hash(&s);
    assert(h3 != h1); // O hash deve mudar

    string_free(&s);
    printf("[OK] Teste de hash e cache concluido.\n");
}

void test_string_contains(void) {
    String main_str = string_create("Programando em C com colecoes modernas");

    // Substring no início
    String sub_start = string_create("Programando");
    assert(string_index_of(&main_str, &sub_start) == 0);

    // Substring no meio
    String sub_mid = string_create("em C");
    assert(string_index_of(&main_str, &sub_mid) == 12);

    // Substring no fim
    String sub_end = string_create("modernas");
    assert(string_index_of(&main_str, &sub_end) == 30);

    // Substring inexistente
    String sub_missing = string_create("Python");
    assert(string_index_of(&main_str, &sub_missing) < 0);

    // Substring maior que a string principal
    String sub_larger = string_create("Programando em C com colecoes modernas e completas");
    assert(string_index_of(&main_str, &sub_larger) < 0);

    // Limpeza
    string_free(&main_str);
    string_free(&sub_start);
    string_free(&sub_mid);
    string_free(&sub_end);
    string_free(&sub_missing);
    string_free(&sub_larger);

    printf("[OK] Teste de string_index_of concluido com sucesso.\n");
}

// Teste 6: Stress e Performance (Benchmark)
void test_string_performance_and_stress(void) {
    printf("[BENCHMARK] Iniciando teste de estresse e performance...\n");
    
    clock_t start = clock();
    
    // Criação e appends em massa (simulando carga real)
    String s = string_create("");
    int iterations = 20000;
    
    for (int i = 0; i < iterations; i++) {
        string_append_char(&s, 'a');
    }
    
    assert(string_length(&s) == iterations);
    
    // Testando velocidade do hash com string grande (com e sem cache)
    clock_t hash_start_1 = clock();
    uint64_t h1 = string_hash(&s); // Primeiro cálculo (pesado)
    clock_t hash_end_1 = clock();

    clock_t hash_start_2 = clock();
    uint64_t h2 = string_hash(&s); // Segundo cálculo (via cache - deve ser instantâneo)
    clock_t hash_end_2 = clock();

    assert(h1 == h2);

    string_free(&s);
    
    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;
    double hash_time_1 = (double)(hash_end_1 - hash_start_1) / CLOCKS_PER_SEC;
    double hash_time_2 = (double)(hash_end_2 - hash_start_2) / CLOCKS_PER_SEC;

    printf(" -> Iteracoes: %d appends\n", iterations);
    printf(" -> Tempo total do teste: %.6f segundos\n", total_time);
    printf(" -> Tempo 1o Hash (sem cache): %.6f seg\n", hash_time_1);
    printf(" -> Tempo 2o Hash (com cache): %.6f seg\n", hash_time_2);
    printf("[OK] Teste de performance e estresse concluido com sucesso!\n");
}

int main(void) {
    printf("--- Executando testes robustos para String ---\n\n");

    test_string_creation_and_getters();
    test_string_equality_and_compare();
    test_string_manipulation();
    test_string_copy_and_clear();
    test_string_hash();
    test_string_contains();
    test_string_performance_and_stress();

    printf("\n--- Todos os testes robustos passaram com sucesso! 🎉 ---\n");
    return 0;
}