#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include "TA.h"

#define d 10

#ifndef BPLUS_H
#define BPLUS_H

typedef struct tabm{
    int eh_folha;//bool
    int num_chaves;//numero de chaves
    long long int ponteiro_pai;//posição do pae
    long long int chaves[2 * d];//cpfs
    long long int filhos[2 * d + 1];//endereço de cada filho
    long long int endereco;//endereço atual
    long long int registros[2 * d];//endereço de todos os elementos presentes no meu nó
    long long int proximo_no;//posição do proxino
} TABM;

#endif

TABM TABM_criar_no();

void TABM_leitura_no(const char* nome_tabm_indices, long long int endereco, TABM* no);
long long int TABM_escreve(long long int endereco_raiz, TA aluno, const char* nome_tabm_indices, const char* nome_tabm_dados);
void TABM_escreve_recursivo(TABM* no, TA data, const char* nome_tabm_indices, const char* data_file);
void TABM_divide(TABM* pai, int indice_filho, TABM* filho, const char* nome_tabm_indices);
void TABM_escreve_no(const char* nome_tabm_indices, TABM no);
long long int TABM_busca_cpf(const char* nome_tabm_indices, long long int cpf, long long int endereco_raiz);
long long int TABM_remove(long long int endereco_raiz, long long int cpf, const char* nome_tabm_indices);
void TABM_balanceia(long long int endereco_no, const char* nome_tabm_indices);
void TABM_remove_ponteiro_pai(TABM pai, int endereco_filho_removido, const char* nome_tabm_indices);
int TABM_remove_chave_no(TABM* no, long long int cpf);
void TABM_busca_e_imprime(long long int endereco_raiz, long long int cpf_alvo, const char* nome_tabm_indices, const char* nome_tab_indices);
void TABM_constroi(FILE *nome_alunos_dados);