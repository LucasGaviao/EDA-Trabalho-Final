//
// Created by Pichau on 11/07/2025.
//

#include "THASH.h"

#include <stdlib.h>
#define TAM 100000

int THASH_h() {
    return rand() % TAM;
}

void THASH_escreve(char *nomeArq, TA *aluno) {
    printf("[THASH_escreve] Comecou!\n");
    FILE *arq = fopen(nomeArq, "rb+");
    if (!arq) {
        printf("Erro ao abrir o arquivo '%s'!\n", nomeArq);
        fclose(arq);
        THASH_inicializa(nomeArq);
        arq = fopen(nomeArq, "rb+");
    }

    srand(aluno->cpf/100);
    int indice = THASH_h();
    TA *registro = TA_inicializa();
    int tentativas = 0;

    fseek(arq, (sizeof(TA) * indice), SEEK_SET);
    TA_leitura(arq, registro);
    while (registro->cpf != -1 && tentativas < TAM) {
        printf("[THASH_escreve] to no while, teve colisão!\n");
        printf("reg:%lld  aluno: %lld\n", registro->cpf, aluno->cpf);

        if (registro->cpf == aluno->cpf) {
            printf("Aluno ja esta na tabela!\n");
            fclose(arq);
            return;
        }

        indice = THASH_h();
        tentativas++;
        fseek(arq, (sizeof(TA) * indice), SEEK_SET);
        TA_leitura(arq, registro);
    }
    printf("indice: %d; cpf: %lld; arq: %p ============================================\n", indice, registro->cpf, arq);

    if (tentativas >= TAM) {
        printf("Nao foi possivel escrever o registro, a hash esta cheia!\n");
        free(registro);
        fclose(arq);
        return;
    }

    printf("[THASH_escreve] escrevendo!\n");
    fseek(arq, (sizeof(TA) * indice), SEEK_SET);
    TA_escrita(arq, aluno);
    printf("[THASH_escreve] acabou com sucesso!\n");
    free(registro);
    fclose(arq);
}

int THASH_busca(char *nomeArq, long long int cpf) {
    /* Retorna -1 caso não funcione ou não ache. Retorna o endereço para leitura do aluno caso ache.*/
    FILE *arq = fopen(nomeArq, "rb");
    if (!arq) {
        printf("Erro ao abrir o arquivo '%s'!\n", nomeArq);
        fclose(arq);
        return -1;
    }
    TA *aluno = TA_inicializa();
    srand(cpf/100);
    int indice = THASH_h();
    int tentativas = 0;
    fseek(arq, (sizeof(TA) * indice), SEEK_SET);
    TA_leitura(arq, aluno);
    while (aluno->cpf != cpf && tentativas < TAM) {
        tentativas++;
        indice = THASH_h();
        fseek(arq, (sizeof(TA) * indice), SEEK_SET);
        TA_leitura(arq, aluno);
    }
    if (aluno->cpf == cpf) {
        printf("Encontramos o Aluno! (cpf: %lld)\n", cpf);
        fclose(arq);
        free(aluno);
        return indice;
    }

    printf("Nao foi possivel encontrar o aluno!\n");
    fclose(arq);
    free(aluno);
    return -1;
}

void THASH_exclui(char *nomeArq, long long int cpf) {
    int indice = THASH_busca(nomeArq, cpf);
    if ( indice == -1) {
        printf("Não foi possivel realizar a remoção!\n");
    } else {
        FILE *arq = fopen(nomeArq, "rb+");
        TA *aluno = TA_inicializa();
        fseek(arq, (sizeof(TA) * indice), SEEK_SET);
        TA_escrita(arq, aluno);
        fclose(arq);
    }
}

void THASH_leitura(char *nomeArq, long long int cpf, TA *aluno) {
    int indice = THASH_busca(nomeArq, cpf);
    if (indice == -1) {
        printf("Não foi possivel realizar leitura!\n");
    } else {
        FILE *arq = fopen(nomeArq, "rb");
        fseek(arq, sizeof(TA) * indice, SEEK_SET);
        TA_leitura(arq, aluno);
    }
}

void THASH_inicializa(char *nomeArq) {
    FILE *arq = fopen(nomeArq, "wb+");
    TA *aluno = TA_inicializa();
    TA_imprime(aluno);
    for (int i = 0; i < TAM; i++) {
        TA_escrita(arq, aluno);
    }
    TA_libera(aluno);
    fclose(arq);
}
