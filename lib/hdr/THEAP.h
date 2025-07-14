//
// Created by lucas on 13/07/2025.
//

#ifndef THEAP_H
#define THEAP_H

#include <stdio.h>
#include <stdlib.h>
#include "TA.h"

int THEAP_pai(int i);
int THEAP_filho_e(int i);
int THEAP_filho_d(int i);
void THEAP_subir(char *nomeHeap, int indice);
void THEAP_descer(char *nomeHeap, int pai_indice, int tam_heap);
int THEAP_busca(char *nomeHeap, long long int cpf);
void THEAP_insere(char *nomeHeap, TA aluno, int tam_heap);
void THEAP_remove(char *nomeHeap, long long int cpf);
int THEAP_verifica(char *nomeHeap);
void THEAP_constroi(char *nomeHeap, char *nomeDados);

#endif //THEAP_H
