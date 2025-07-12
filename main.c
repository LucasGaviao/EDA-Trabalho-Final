#include <stdio.h>
#include "lib/gerar10k.h"
#include "lib/testehash.h"

#define  K10 10000
int main(void) {
    teste_hash();

    // FILE *fonte = fopen("10kaluno.bin", "rb");
    // FILE *hash = fopen("hash.bin", "rb");
    // if (!fonte) {
    //     printf("não foi possivel acessar o arquivo!\n");
    //     exit(1);
    // }
    //
    // TA *aluno = TA_inicializa();
    // TA *aluno_h = TA_inicializa();
    // for (int i = 0; i < K10; i++) {
    //     TA_leitura(fonte, aluno);
    //     THASH_leitura("hash.bin", aluno->cpf, aluno_h);
    //     if (aluno->cpf == aluno_h->cpf) {
    //         printf("Esta na hash e o cpf é igual!\n");
    //     } else {
    //         printf("Deu alguma merda!\n");
    //     }
    // }
    return 0;
}