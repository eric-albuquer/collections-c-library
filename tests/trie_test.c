#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "collections.h" // Inclui o seu cabeçalho atualizado

void test_trie_basic() {
    printf("Executando test_trie_basic...\n");

    Trie trie = trie_create();
    assert(trie_length(&trie) == 0);

    // Adicionando strings em formato C (char*)
    trie_add_str(&trie, "casa");
    trie_add_str(&trie, "casamento");
    trie_add_str(&trie, "carro");

    // Valida o número de palavras inseridas
    assert(trie_length(&trie) == 3);

    // Testa buscas por palavras completas existentes
    assert(trie_has_str(&trie, "casa") == true);
    assert(trie_has_str(&trie, "casamento") == true);
    assert(trie_has_str(&trie, "carro") == true);

    // Testa buscas por palavras que não existem ou são parciais
    assert(trie_has_str(&trie, "cas") == false);      // Prefixo, mas não palavra completa
    assert(trie_has_str(&trie, "cartao") == false);   // Inexistente
    assert(trie_has_str(&trie, "casamentos") == false); // Sobra de caracteres

    // Testa verificação de prefixos
    assert(trie_has_prefix_str(&trie, "cas") == true);
    assert(trie_has_prefix_str(&trie, "ca") == true);
    assert(trie_has_prefix_str(&trie, "car") == true);
    assert(trie_has_prefix_str(&trie, "cachorro") == false);

    trie_free(&trie);
    printf("test_trie_basic passou com sucesso!\n");
}

void test_trie_with_string_struct() {
    printf("Executando test_trie_with_string_struct...\n");

    Trie trie = trie_create();

    // Criando objetos String da sua lib
    String s1 = string_create("programacao");
    String s2 = string_create("progresso");
    String s3 = string_create("pro");

    trie_add_string(&trie, &s1);
    trie_add_string(&trie, &s2);
    trie_add_string(&trie, &s3);

    assert(trie_length(&trie) == 3);

    // Testa busca usando structs String
    assert(trie_has_string(&trie, &s1) == true);
    assert(trie_has_string(&trie, &s2) == true);
    assert(trie_has_string(&trie, &s3) == true);

    // Testa prefixos usando structs String
    String p1 = string_create("progr");
    String p2 = string_create("avi");

    assert(trie_has_prefix_string(&trie, &p1) == true);
    assert(trie_has_prefix_string(&trie, &p2) == false);

    // Limpeza das strings e da trie
    string_free(&s1);
    string_free(&s2);
    string_free(&s3);
    string_free(&p1);
    string_free(&p2);
    trie_free(&trie);

    printf("test_trie_with_string_struct passou com sucesso!\n");
}

void test_trie_edge_cases() {
    printf("Executando test_trie_edge_cases...\n");

    Trie trie = trie_create();

    // Inserção de string vazia (dependendo da sua implementação, pode ser ignorada ou tratada)
    trie_add_str(&trie, "");
    assert(trie_has_str(&trie, "") == true);

    // Inserção de palavras duplicadas
    trie_add_str(&trie, "teste");
    trie_add_str(&trie, "teste");
    assert(trie_length(&trie) == 2); // Não deve duplicar contagem se a palavra já existe
    assert(trie_has_str(&trie, "teste") == true);

    trie_free(&trie);
    printf("test_trie_edge_cases passou com sucesso!\n");
}

int main() {
    printf("=== INICIANDO TESTES DA TRIE ===\n\n");
    
    test_trie_basic();
    printf("\n");
    
    test_trie_with_string_struct();
    printf("\n");
    
    test_trie_edge_cases();
    
    printf("\n=== TODOS OS TESTES DA TRIE PASSARAM COM SUCESSO! ===\n");
    return 0;
}