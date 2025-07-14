// THEAP.c

#include "../hdr/THEAP.h"
#include <string.h> // Para memset

#define NUM_ALUNOS 10000


int THEAP_pai(int i) {
    if (i <= 0) return -1;
    return (i - 1) / 2;
}

int THEAP_filho_e(int i) {
    return 2 * i + 1;
}

int THEAP_filho_d(int i) {
    return 2 * i + 2;
}

void THEAP_guarda_tam(long int *tam_heap){
    FILE *reg_heap = fopen("reg_heap.bin", "wb");
    if (!reg_heap) {
        printf("[THEAP_guarda_tamanho] Impossivel criar 'reg_heap.bin'!\n");
        exit(1);
    }
    fwrite(tam_heap, sizeof(long), 1, reg_heap);
    fclose(reg_heap);
}

int THEAP_retorna_tam(long int *tam_heap) {
    FILE *reg_heap = fopen("reg_heap.bin", "rb");
    if (!reg_heap) {
        printf("[THEAP_retorna_tamanho] Impossivel ler 'reg_heap.bin'!\n");
        return 0;
    }
    fread(tam_heap, sizeof(long), 1, reg_heap);
    return 1;
}

// --- Funções de Manutenção (com I/O abstraído) ---

void THEAP_subir(FILE *heap, int indice, long int *tam_heap) {
    if (indice <= 0) return;
    int pai_indice = THEAP_pai(indice);
    TA filho, pai;


    fseek(heap, indice * sizeof(TA), SEEK_SET);
    TA_leitura(heap, &filho); // Substituído fread


    fseek(heap, pai_indice * sizeof(TA), SEEK_SET);
    TA_leitura(heap, &pai); // Substituído fread


    if (filho.nota >= pai.nota) {

        fseek(heap, pai_indice * sizeof(TA), SEEK_SET);
        TA_escrita(heap, &filho); // Substituído fwrite

        fseek(heap, indice * sizeof(TA), SEEK_SET);
        TA_escrita(heap, &pai); // Substituído fwrite

        fflush(heap);
        THEAP_subir(heap, pai_indice, tam_heap);
    }
}

void THEAP_descer(FILE *heap, int indice, long int *tam_heap) {
    int filho_e_indice = THEAP_filho_e(indice);
    int filho_d_indice = THEAP_filho_d(indice);
    int maior_indice = indice;

    TA pai, filho_e, filho_d, temp;


    fseek(heap, indice * sizeof(TA), SEEK_SET);
    TA_leitura(heap, &pai);

    if (filho_e_indice < *tam_heap) {
        fseek(heap, filho_e_indice * sizeof(TA), SEEK_SET);
        TA_leitura(heap, &filho_e);
        if (filho_e.nota > pai.nota) {
            maior_indice = filho_e_indice;
        }
    }

    if (filho_d_indice < *tam_heap) {
        TA maior_atual;
        fseek(heap, maior_indice * sizeof(TA), SEEK_SET);
        TA_leitura(heap, &maior_atual);

        fseek(heap, filho_d_indice * sizeof(TA), SEEK_SET);
        TA_leitura(heap, &filho_d);

        if (filho_d.nota > maior_atual.nota) {
            maior_indice = filho_d_indice;
        }
    }

    if (maior_indice != indice) {
        fseek(heap, maior_indice * sizeof(TA), SEEK_SET);
        TA_leitura(heap, &temp);

        fseek(heap, indice * sizeof(TA), SEEK_SET);
        TA_escrita(heap, &temp);

        fseek(heap, maior_indice * sizeof(TA), SEEK_SET);
        TA_escrita(heap, &pai);

        THEAP_descer(heap, maior_indice, tam_heap);
    }
}

void THEAP_escreve(char *nomeHeap, TA *aluno, long int *tam_heap) {
    FILE *heap = fopen(nomeHeap, "rb+");
    if (!heap) {
        printf("Erro na inserção. Não foi possível abrir o arquivo.\n");
        return;
    }
    fseek(heap, (*tam_heap) * sizeof(TA), SEEK_SET);
    TA_escrita(heap, aluno);

    (*tam_heap)++;
    THEAP_subir(heap, *tam_heap-1, tam_heap);
    THEAP_guarda_tam(tam_heap);
    fclose(heap);
}

void THEAP_exclui(char *nomeHeap, long int *tam_heap) {
    FILE *heap = fopen(nomeHeap, "rb+");
    if (!heap) {
        printf("Erro na remoção. Não foi possível abrir o arquivo.\n");
        return;
    }

    if (*tam_heap == 0) {
        fclose(heap);
        return;
    }

    // ler primeiro elemento para o print
    fseek(heap, 0, SEEK_SET);
    TA removido;
    TA_leitura(heap, &removido);
    TA_imprime(&removido);

    // Remoção
    TA ultimo_aluno;
    fseek(heap, (*tam_heap -1) * sizeof(TA), SEEK_SET);
    TA_leitura(heap, &ultimo_aluno);

    fseek(heap, 0, SEEK_SET);
    TA_escrita(heap, &ultimo_aluno);

    (*tam_heap)--;
    THEAP_descer(heap, 0, tam_heap);

    fclose(heap);
    THEAP_guarda_tam(tam_heap);
    printf("Aluno: %s, nota:%d, cpf:%lld, indice:%d. REMOVIDO!\n",removido.nome, removido.nota, removido.cpf, 0);
}

TA* THEAP_busca(char *nomeHeap, long long int cpf, long int *tam_heap) {
    FILE *heap = fopen(nomeHeap, "rb");
    if (!heap) {
        return NULL;
    }

    TA *aluno = TA_inicializa();
    int contador = 0;
    while(TA_leitura(heap, aluno) == 1 && contador < *tam_heap) {
        if (aluno->cpf == cpf) {
            fclose(heap);
            return aluno;
        }
    }

    free(aluno);
    fclose(heap);
    return NULL;
}


void THEAP_constroi(char *nomeHeap, char *nomeDados, long int *tam_heap) {
    FILE *dados = fopen(nomeDados, "rb");
    if (!dados) return;

    FILE *heap = fopen(nomeHeap, "wb+");
    if (!heap) {
        fclose(dados);
        return;
    }

    TA aluno_temp;
    while(TA_leitura(dados, &aluno_temp) == 1) {
        TA_escrita(heap, &aluno_temp);
    }
    fclose(dados);

    fseek(heap, 0, SEEK_END);
    (*tam_heap) = ftell(heap) / sizeof(TA);
    for (int i = (*tam_heap / 2) - 1; i >= 0; i--) {
        THEAP_descer(heap, i, tam_heap);
    }
    THEAP_guarda_tam(tam_heap);
    fclose(heap);
}


int THEAP_verifica(char *nomeHeap) {
    FILE *heap = fopen(nomeHeap, "rb");
    if (!heap) return 0;
    long int tam_heap;
    THEAP_retorna_tam(&tam_heap);

    for (int i = 0; i < tam_heap; i++) {
        TA pai, filho_e, filho_d;

        fseek(heap, i * sizeof(TA), SEEK_SET);
        TA_leitura(heap, &pai);

        int filho_e_idx = THEAP_filho_e(i);
        if (filho_e_idx < tam_heap) {
            fseek(heap, filho_e_idx * sizeof(TA), SEEK_SET);
            TA_leitura(heap, &filho_e);
            if (filho_e.nota > pai.nota) {
                printf("Erro de Max-Heap: Pai na pos %d (nota %d) < Filho esquerdo (nota %d)\n", i, pai.nota, filho_e.nota);
                fclose(heap);
                return 0;
            }
        }

        int filho_d_idx = THEAP_filho_d(i);
        if (filho_d_idx < tam_heap) {
            fseek(heap, filho_d_idx * sizeof(TA), SEEK_SET);
            TA_leitura(heap, &filho_d);
            if (filho_d.nota > pai.nota) {
                printf("Erro de Max-Heap: Pai na pos %d (nota %d) < Filho direito (nota %d)\n", i, pai.nota, filho_d.nota);
                fclose(heap);
                return 0;
            }
        }
    }
    fclose(heap);
    return 1;
}