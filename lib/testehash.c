//
// Created by Pichau on 11/07/2025.
//

#include "testehash.h"
#define K10 10000

void teste_hash() {
    FILE *fonte = fopen("10kaluno.bin", "rb");
    if (!fonte) {
        printf("não foi possivel acessar o arquivo!\n");
        exit(1);
    }
    THASH_inicializa("hash.bin");

    TA *aluno = TA_inicializa();
    TA *aluno_h = TA_inicializa();
    for (int i = 0; i < 10; i++) {
        TA_leitura(fonte, aluno);
        TA_imprime(aluno);
        printf("%lld", aluno->cpf);

        THASH_escreve("hash.bin", aluno);
        THASH_leitura("hash.bin", aluno->cpf, aluno_h);
        TA_imprime(aluno_h);
    }

    fclose(fonte);
    TA_libera(aluno);
}