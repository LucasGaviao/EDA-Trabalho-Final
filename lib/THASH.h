//
// Created by Pichau on 11/07/2025.
//

#ifndef THASH_H
#define THASH_H

#include "TA.h"
int THASH_h(long long int cpf, int k);
int THASH_ocupado(FILE *arq);
int THASH_busca(char *nomeArq, long long int cpf);
void THASH_escreve(char *nomeArq, TA *aluno);
void THASH_leitura(char *nomeArq, long long int cpf, TA *aluno);
void THASH_constroi(char *nomeHash, char *nomeDados);
#endif //THASH_H
