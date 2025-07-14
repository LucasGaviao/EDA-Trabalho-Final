// THEAP.h

#ifndef THEAP_H
#define THEAP_H

#include <stdio.h>
#include <stdlib.h>
#include "TA.h"

int THEAP_pai(int i);
int THEAP_filho_e(int i);
int THEAP_filho_d(int i);


void THEAP_subir(FILE *heap, int indice, long int *tam_heap);
void THEAP_descer(FILE *heap, int indice, long int *tam_heap);


void THEAP_escreve(char *nomeHeap, TA *aluno, long int *tam_heap);
void THEAP_exclui(char *nomeHeap, long int *tam_heap);
TA* THEAP_busca(char *nomeHeap, long long int cpf, long int *tam_heap);


void THEAP_constroi(char *nomeHeap, char *nomeDados, long int *tam_heap);
int THEAP_verifica(char *nomeHeap);

#endif //THEAP_H