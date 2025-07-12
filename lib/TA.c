//
// Created by Pichau on 11/07/2025.
//

#include "TA.h"

TA *TA_inicializa() {
    TA *novo = (TA *)malloc(sizeof(TA));
    for (int i = 0; i < 49; i++) novo->nome[i] = ' ';
    novo->nome[49] = '\0';
    novo->cpf = -1;
    novo->nota = -1;
    return novo;
}

void TA_set_nome(TA *aluno, char *nome) {
    int n = strlen(nome);
    for (int i = 0; i < n; i++) aluno->nome[i] = nome[i];
}
void TA_set_cpf(TA *aluno, long long int cpf) {
    aluno->cpf = cpf;
}

void TA_set_nota(TA *aluno, int nota) {
    aluno->nota = nota;
}

void TA_libera(TA *aluno) {
    free(aluno);
}

void TA_leitura(FILE *arq, TA *aluno) {
    fread(aluno->nome, sizeof(char), 50, arq);
    fread(&aluno->cpf, sizeof(long long int), 1, arq);
    fread(&aluno->nota, sizeof(int), 1, arq);
}

void TA_escrita(FILE *arq, TA *aluno) {
    fwrite(aluno->nome, sizeof(char), 50, arq);
    fwrite(&aluno->cpf, sizeof(long long int), 1, arq);
    fwrite(&aluno->nota, sizeof(int), 1, arq);
}

void TA_imprime(TA *aluno) {
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    printf("Aluno: %s\n", aluno->nome);
    printf("CPF: %lld\n", aluno->cpf);
    printf("Nota: %d\n", aluno->nota);
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}