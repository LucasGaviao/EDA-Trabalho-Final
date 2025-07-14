// TABM.h

#ifndef TABM_H
#define TABM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TA.h" // PADRONIZAÇÃO: Usando a estrutura de Aluno padrão do projeto

#define TAM 100 // Ordem da árvore (usando um nome mais claro)

// PADRONIZAÇÃO: Cabeçalho para gerenciar o estado do arquivo de índice
typedef struct {
    long long endereco_raiz;
    long long proximo_endereco_livre;
} Cabecalho;

// PADRONIZAÇÃO: Estrutura do nó renomeada para TABM e com chave long long int
typedef struct {
    int eh_folha;
    int num_chaves;
    long long int ponteiro_pai;
    long long int chaves[TAM * 2];
    long long int filhos[TAM * 2 + 1];
    long long int endereco;
    long long int reg[TAM * 2];
    long long int proximo_no;
} TABM;


void TARVBP_reseta_contador_nos();
long long TARVBP_insere(long long int endereco_raiz, TA* d, const char* nome_tabm_indices, const char* nome_tabm_dados);
long long busca_cpf(const char* nome_tabm_indices, long long int cpf, long long int endereco_raiz);
long long TARVBP_remove(long long int endereco_raiz, long long int cpf, const char* nome_tabm_indices);
void buscar_e_imprimir_dados(long long int endereco_raiz, long long int cpf_busca, const char* nome_tabm_indices, const char* arq_dados_nome);
long long constroi_arvore(const char* nome_alunos_dados, const char* nome_tabm_indices, const char* nome_tabm_dados);

#endif //TABM_H