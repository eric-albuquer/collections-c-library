#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "collections.h" // Inclui o seu cabeçalho atualizado

void test_bitset_basic() {
    printf("Executando test_bitset_basic...\n");

    // Cria um bitset com capacidade para 128 bits (cobre exatamente 2 blocos de uint64_t)
    BitSet bs = bitset_create(128);

    // Inicialmente todos devem estar desligados (false)
    assert(bitset_none(&bs) == true);
    assert(bitset_any(&bs) == false);
    assert(bitset_all(&bs) == false);

    // Testa definição e leitura de bits individuais
    bitset_set(&bs, 0);   // Primeiro bit do bloco 0
    bitset_set(&bs, 63);  // Último bit do bloco 0
    bitset_set(&bs, 64);  // Primeiro bit do bloco 1
    bitset_set(&bs, 127); // Último bit do bloco 1

    assert(bitset_get(&bs, 0) == true);
    assert(bitset_get(&bs, 32) == false); // Não foi ativado
    assert(bitset_get(&bs, 63) == true);
    assert(bitset_get(&bs, 64) == true);
    assert(bitset_get(&bs, 127) == true);

    // Testa o flip (inversão)
    bitset_flip(&bs, 32); // Deve virar true
    assert(bitset_get(&bs, 32) == true);
    
    bitset_flip(&bs, 0);  // Deve virar false
    assert(bitset_get(&bs, 0) == false);

    // Testa verificação de estado geral
    assert(bitset_any(&bs) == true);
    assert(bitset_none(&bs) == false);

    // Testa clear (reseta tudo para 0)
    bitset_clear(&bs);
    assert(bitset_none(&bs) == true);
    assert(bitset_get(&bs, 64) == false);

    bitset_free(&bs);
    printf("test_bitset_basic passou com sucesso!\n");
}

void test_bitset_fill() {
    printf("Executando test_bitset_fill...\n");

    BitSet bs = bitset_create(100);

    // Preenche tudo com true (1)
    bitset_fill(&bs, true);
    assert(bitset_all(&bs) == true);
    assert(bitset_get(&bs, 0) == true);
    assert(bitset_get(&bs, 99) == true);

    // Preenche tudo com false (0)
    bitset_fill(&bs, false);
    assert(bitset_none(&bs) == true);

    bitset_free(&bs);
    printf("test_bitset_fill passou com sucesso!\n");
}

void test_bitset_logic_operations() {
    printf("Executando test_bitset_logic_operations...\n");

    BitSet a = bitset_create(64);
    BitSet b = bitset_create(64);

    // Configura 'a' com os bits 0 e 1 ligados
    bitset_set(&a, 0);
    bitset_set(&a, 1);

    // Configura 'b' com os bits 1 e 2 ligados
    bitset_set(&b, 1);
    bitset_set(&b, 2);

    // Teste AND (esperado: apenas o bit 1 ligado)
    BitSet res_and = bitset_and(&a, &b);
    assert(bitset_get(&res_and, 0) == false);
    assert(bitset_get(&res_and, 1) == true);
    assert(bitset_get(&res_and, 2) == false);
    bitset_free(&res_and);

    // Teste OR (esperado: bits 0, 1 e 2 ligados)
    BitSet res_or = bitset_or(&a, &b);
    assert(bitset_get(&res_or, 0) == true);
    assert(bitset_get(&res_or, 1) == true);
    assert(bitset_get(&res_or, 2) == true);
    assert(bitset_get(&res_or, 3) == false);
    bitset_free(&res_or);

    // Teste XOR (esperado: bits 0 e 2 ligados, bit 1 cancelado)
    BitSet res_xor = bitset_xor(&a, &b);
    assert(bitset_get(&res_xor, 0) == true);
    assert(bitset_get(&res_xor, 1) == false);
    assert(bitset_get(&res_xor, 2) == true);
    bitset_free(&res_xor);

    // Teste NOT (inverte todos os bits de 'a')
    BitSet res_not = bitset_not(&a);
    assert(bitset_get(&res_not, 0) == false);
    assert(bitset_get(&res_not, 1) == false);
    assert(bitset_get(&res_not, 2) == true); // O que era 0 agora é 1
    bitset_free(&res_not);

    // Teste de Cópia (copy)
    BitSet a_copy = bitset_copy(&a);
    assert(bitset_get(&a_copy, 0) == true);
    assert(bitset_get(&a_copy, 1) == true);
    assert(bitset_get(&a_copy, 2) == false);
    bitset_free(&a_copy);

    bitset_free(&a);
    bitset_free(&b);

    printf("test_bitset_logic_operations passou com sucesso!\n");
}

int main() {
    printf("=== INICIANDO TESTES DO BITSET ===\n\n");

    test_bitset_basic();
    printf("\n");

    test_bitset_fill();
    printf("\n");

    test_bitset_logic_operations();

    printf("\n=== TODOS OS TESTES DO BITSET PASSARAM COM SUCESSO! ===\n");
    return 0;
}