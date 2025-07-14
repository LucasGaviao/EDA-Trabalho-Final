//
// Created by Pichau on 11/07/2025.
//

#include "testehash.h"
#define K10 10000

int teste_ler_10k(char *nomeHash, char *nomeDados) {
    FILE *alunos = fopen(nomeDados, "rb");
    if (alunos == NULL) {
        printf("[teste_ler_10k]Erro ao abrir %s! \n", nomeDados);
        exit(1);
    }
    int contador = 0;
    for (int i = 0; i < K10 && !feof(alunos); i++) {
        TA aluno_f;
        TA aluno_h;
        if (TA_leitura(alunos, &aluno_f) != 1) {
            printf("[teste_ler_10k]Erro ao realizar a leitura em %s!\n", nomeDados);
            return -1;
        }
        if (aluno_f.cpf < 0) {
            printf("[teste_ler_10k]Erro! Dado corrompido!\n");
            return -1;
        }

        contador += THASH_leitura(nomeHash, aluno_f.cpf, &aluno_h);
        TA_imprime(&aluno_h);
    }
    if (contador >= K10) {
        printf("[teste_ler_10k]Todas as operacoes foram realizadas! %d.\n", contador);
        return 1;
    }
    printf("[teste_ler_10k]Operacoes corrompidas! %d.\n",K10-contador);
    return 0;
}

void teste_hash(char *nomeHash, char *nomeDados) {
    THASH_constroi(nomeHash, nomeDados);
}