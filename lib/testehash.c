//
// Created by Pichau on 11/07/2025.
//

#include "testehash.h"
#define K10 10000

void teste_cria_hash(char *nomeArq) {
    FILE *arq = fopen(nomeArq, "wb");
    if (arq == NULL) {
        printf("Erro ao criar o %s!",nomeArq);
        exit(1);
    }
    TA *aluno = TA_inicializa();
    for (int i = 0; i < 100000; i++) {
        fwrite(aluno, sizeof(TA), 1, arq);
    }

    free(aluno);
    fclose(arq);
}

int teste_ler_10k(char *nomeHash, char *nomeDados) {
    FILE *alunos = fopen(nomeDados, "rb");
    if (alunos == NULL) {
        printf("[teste_ler_10k]Erro ao abrir %s! \n", nomeDados);
        exit(1);
    }

    for (int i = 1; i < K10 && !feof(alunos); i++) {
        TA *aluno_f = TA_inicializa();
        TA *aluno_h = TA_inicializa();
        TA_leitura(alunos, aluno_f);
        if (aluno_f == NULL) {
            printf("[teste_ler_10k]Erro ao realizar a leitura em %s!\n", nomeDados);
            return -1;
        }
        if (aluno_f->cpf < 0) {
            printf("[teste_ler_10k]Erro! Dado corrompido!\n");
            return -1;
        }

        THASH_leitura(nomeHash, aluno_f->cpf, aluno_h);

        free(aluno_f);
        free(aluno_h);
    }
    printf("[teste_ler_10k]Todas as operações foram realizadas!");
    return 1;
}

void teste_hash(char *nomeHash, char *nomeDados) {
    THASH_constroi(nomeHash, nomeDados);

}