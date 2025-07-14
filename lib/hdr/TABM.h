// TABM.h (Refatorado)

#ifndef TABM_H
#define TABM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TA.h" // Usando a estrutura de Aluno já existente

#define TAM_TABM 100 // Ordem da árvore

// Cabeçalho do arquivo de índice para guardar metadados
typedef struct {
    long long endereco_raiz;
    long long proximo_endereco_livre;
} Cabecalho;

// Estrutura do nó da Árvore-B
typedef struct {
    int eh_folha;
    int num_chaves;
    long long int chaves[TAM_TABM * 2 - 1];
    long long filhos[TAM_TABM * 2];
    long long endereco;
    long long reg[TAM_TABM * 2 - 1];
    long long proximo_no;
} TABM;

// --- Protótipos das Funções Públicas ---

// Funções de manipulação do arquivo e nós (internas, mas úteis para depuração)
void TABM_ler_no(const char* arq_idx, long long endereco, TABM* no);
void TABM_escrever_no(const char* arq_idx, TABM* no);

// Funções principais da Árvore-B (a verdadeira API pública)
void TABM_inicializa(const char* arq_idx_nome);
void TABM_insere(const char* arq_idx_nome, const char* arq_dados_nome, TA* aluno);
void TABM_remove(const char* arq_idx_nome, long long int cpf);
long long TABM_busca_cpf(const char* arq_idx_nome, long long int cpf);

// Os protótipos das funções recursivas e de divisão foram removidos
// por serem detalhes de implementação interna do módulo.

#endif //TABM_H