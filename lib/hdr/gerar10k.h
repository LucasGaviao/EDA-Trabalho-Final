//
// Created by Pichau on 11/07/2025.
//

#ifndef GERAR10K_H
#define GERAR10K_H

#include <stdio.h>
#include <stdlib.h>
#include "TA.h"
#include <time.h>

int gerar_nota();
long long int gerar_cpf ();
void atualiza_nome(char *nome, int num);
int gerar_10k(char *nomeDados, long long int* all);
int ler_10k(char *nomeDados);

#endif //GERAR10K_H
