//
// Created by Pichau on 11/07/2025.
//

#include "THASH.h"

#include <stdlib.h>
#define TAM 100000

int THASH_h() {
    return rand() % TAM;
}

int THASH_ocupado(FILE *arq) {
    FILE *atual = arq;
    int ocupado = 0;
    printf("[THASH_ocupado]chegou ate aqui!\n");
    TA *aluno = TA_inicializa();
    fread(aluno, sizeof(TA), 1, atual);
    if (aluno->cpf != -1) {
        printf("[THASH_ocupado] Posição ocupada!\n");
        TA_imprime(aluno);
        ocupado = 1;
        printf("[THASH_ocupado] Saiu com 1!\n");
        free(aluno);
        return ocupado;
    }
    free(aluno);
    printf("[THASH_ocupado] Saiu com 0!\n");
    return ocupado;
}

int THASH_insere_aux(FILE *arq, TA *alvo) {
    FILE *atual = arq;
    printf("[aux]ENTROU NESTE CARALHO\n");
    TA *registro = TA_inicializa();
    fread(registro, sizeof(TA), 1, atual);
    if (registro->cpf == alvo->cpf) return 1;
    printf("[aux]RETORNOU 0 CARALHO\n");
    return 0;
}

void THASH_escreve(char *nomeArq, TA *aluno) {
    printf("[THASH_escreve] Comecou!\n");
    FILE *arq = fopen(nomeArq, "rb+");
    if (!arq) {
        printf("Erro ao abrir o arquivo '%s'!\n", nomeArq);
        fclose(arq);
        arq = fopen(nomeArq, "wb+");
    }

    srand(aluno->cpf/100);
    int indice = THASH_h();
    fseek(arq, (sizeof(TA) * indice), SEEK_SET);
    int ocupado = THASH_ocupado(arq);
    while (ocupado) {
        printf("[THASH_escreve] to no while, teve colisão!\n");
        if (THASH_insere_aux(arq, aluno)) {
            printf("Aluno ja esta na tabela!\n");
            fclose(arq);
            return;
        }
        indice = THASH_h();
        fseek(arq, (sizeof(TA) * indice), SEEK_SET);
        ocupado = THASH_ocupado(arq);
        printf("indice: %d; ocupado: %d; arq: %p ============================================\n", indice, ocupado, arq);
    }
    printf("indice: %d; ocupado: %d; arq: %p ============================================\n", indice, ocupado, arq);

    printf("[THASH_escreve] escrevendo!\n");
    TA_escrita(arq, aluno);
    printf("[THASH_escreve] acabou com sucesso!\n");
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

    fseek(arq, (sizeof(TA) * indice), SEEK_SET);
    while (aluno->cpf != cpf) {
        if (THASH_ocupado(arq)) {
            fseek(arq, sizeof(int), SEEK_CUR);
            TA_leitura(arq, aluno);
            if (aluno->cpf == cpf) {
                printf("Encontramos o Aluno! (cpf: %lld)\n", cpf);
                return indice;
            }
        }
        indice = THASH_h(aluno->cpf);
        fseek(arq, (sizeof(TA) * indice), SEEK_SET);
    }

    fclose(arq);
    return -1;
}

void THASH_exclui(char *nomeArq, long long int cpf) {
    int indice = THASH_busca(nomeArq, cpf);
    if ( indice == -1) {
        printf("Não foi possivel realizar a remoção!\n");
    } else {
        FILE *arq = fopen(nomeArq, "rb");
        TA *aluno = TA_inicializa();
        fseek(arq, (sizeof(TA) * indice), SEEK_SET);
        fwrite(aluno, sizeof(int), 1, arq);
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
        fwrite(aluno, sizeof(TA), 1, arq);
    }
    TA_libera(aluno);
    fclose(arq);
}
