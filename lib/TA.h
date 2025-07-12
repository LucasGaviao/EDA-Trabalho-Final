//
// Created by Pichau on 11/07/2025.
//

#ifndef TA_H
#define TA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct aluno{
    char nome[50];
    long long int cpf;
    int nota;
}TA;

TA *TA_inicializa();
void TA_set_nome(TA *aluno, char *nome);
void TA_set_cpf(TA *aluno, long long int cpf);
void TA_set_nota(TA *aluno, int nota);
void TA_libera(TA *aluno);
void TA_leitura(FILE *arq, TA *aluno);
void TA_escrita(FILE *arq, TA *aluno);
void TA_imprime(TA *aluno);


#endif //TA_H
