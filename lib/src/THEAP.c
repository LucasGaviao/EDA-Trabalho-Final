//
// Created by lucas on 13/07/2025.
//

#include "../hdr/THEAP.h"

#define aluno_size 10000

int THEAP_pai(int i){
    if (i <= 0) return -1;
    return (i - 1)/2;
}

int THEAP_filho_e(int i){
    return 2*i + 1;
}

int THEAP_filho_d(int i){
    return 2*i + 2;
}

void THEAP_subir(char *nomeHeap, int indice){
    FILE *heap = fopen(nomeHeap, "rb+");
    if (!heap) {
        printf("Erro ao tentar subir. Arquivo inválido.\n");
        exit(1);
    }

    TA filho;
    fseek(heap, (sizeof(TA) * indice), SEEK_SET);
    TA_leitura(heap, &filho);

    int pai_indice = THEAP_pai(indice);
    if(pai_indice >= 0){
        TA pai;
        fseek(heap, (sizeof(TA) * pai_indice), SEEK_SET);
        TA_leitura(heap, &pai);

        if(filho.nota >= pai.nota){
            fseek(heap, (sizeof(TA) * pai_indice), SEEK_SET);
            TA_escrita(heap, &filho);
            fflush(heap);

            fseek(heap, (sizeof(TA) * indice), SEEK_SET);
            TA_escrita(heap, &pai);
            fflush(heap);

            fclose(heap);
            THEAP_subir(nomeHeap, pai_indice);
        }
    }
    fclose(heap);
}

void THEAP_descer(char *nomeHeap, int pai_indice, int heap_tam){
    FILE *heap = fopen(nomeHeap, "rb+");
    if (!heap) {
        printf("Erro ao tentar descer. Arquivo inválido.\n");
        exit(1);
    }
    rewind(heap);

    TA pai;
    fseek(heap, (sizeof(TA) * pai_indice), SEEK_SET);
    TA_leitura(heap, &pai);

    TA filho_d;
    int filho_d_indice = THEAP_filho_d(pai_indice);
    if(filho_d_indice){
        fseek(heap, (sizeof(TA) * filho_d_indice), SEEK_SET);
        TA_leitura(heap, &filho_d);
    }

    TA filho_e;
    int filho_f_indice = THEAP_filho_e(pai_indice);
    if(filho_f_indice < heap_tam){
        fseek(heap, (sizeof(TA) * filho_f_indice), SEEK_SET);
        TA_leitura(heap, &filho_e);
    }

    if(pai.nota < filho_d.nota && filho_d.nota >= filho_e.nota){
        fseek(heap, (sizeof(TA) * pai_indice), SEEK_SET);
        fwrite(&filho_d, sizeof(TA), 1, heap);
        fseek(heap, (sizeof(TA) * filho_d_indice), SEEK_SET);
        fwrite(&pai, sizeof(TA), 1, heap);

        fclose(heap);
        THEAP_descer(heap, filho_d_indice, heap_tam);
    } else if(pai.nota < filho_e.nota && filho_e.nota > filho_d.nota){
        fseek(heap, (sizeof(TA) * pai_indice), SEEK_SET);
        fwrite(&filho_e, sizeof(TA), 1, heap);
        fseek(heap, (sizeof(TA) * filho_f_indice), SEEK_SET);
        fwrite(&pai, sizeof(TA), 1, heap);

        fclose(heap);
        THEAP_descer(heap, filho_f_indice, heap_tam);
    }
}

int THEAP_busca(char *nomeHeap, long long int cpf){
    FILE *arq = fopen(nomeHeap, "rb+");
    if (!arq) {
        printf("Erro na busca. Não foi possível abrir o arquivo.\n");
        return -1;
    }
    rewind(arq);

    TA target;

    fseek(arq, 0, SEEK_END);
    long heap_tam = ftell(arq)/sizeof(TA);

    if(heap_tam < 0){
        printf("Erro na busca. Heap vazia ou inexistente.\n");
        return -1;
    }
    for(int i = 0; i < heap_tam; i++){
        fseek(arq, sizeof(TA) * i, SEEK_SET);
        TA_leitura(arq, &target);
        if(target.cpf == cpf){
            return i;
        }
    }

    printf("Erro na busca. Estudante inexistente.\n");
    return -1;
}

void THEAP_insere(char *nomeHeap, TA aluno, int heap_tam){
    FILE *heap = fopen(nomeHeap, "rb+");
    if (!heap) {
        printf("Erro na inserção. Não foi possível abrir o arquivo.\n");
        return;
    }
    rewind(heap);

    fseek(heap, 0, SEEK_END);
    fwrite(&aluno, sizeof(TA), 1, heap);

    if(heap_tam == -1) heap_tam = ftell(heap)/sizeof(TA);
    fclose(heap);
    THEAP_subir(heap, heap_tam);
}

void THEAP_remove(char *nomeHeap, long long int cpf){
    FILE *arq = fopen(nomeHeap, "rb+");
    if (!arq) {
        printf("Erro na remoção. Não foi possível abrir o arquivo.\n");
        return;
    }
    fseek(arq, sizeof(TA), SEEK_SET);

}

int THEAP_verifica(char *nomeHeap) {
    FILE *arq = fopen(nomeHeap, "rb+");
    if (!arq) return 0;

    fseek(arq, 0, SEEK_END);
    long size = ftell(arq) / sizeof(TA);

    for (int i = 0; i < size; i++) {
        TA pai, filho_e, filho_d;
        fseek(arq, sizeof(TA) * i, SEEK_SET);
        TA_leitura(arq, &pai);

        int filho_e_indice = THEAP_filho_e(i);
        int filho_d_indice = THEAP_filho_d(i);

        if (filho_e_indice < size) {
            fseek(arq, sizeof(TA) * filho_e_indice, SEEK_SET);
            TA_leitura(arq, &filho_e);
            if (filho_e.nota > pai.nota) {
                printf("Erro: filho esquerdo maior que pai na pos %d\n", i);
                return 0;
            }
        }

        if (filho_d_indice < size) {
            fseek(arq, sizeof(TA) * filho_d_indice, SEEK_SET);
            TA_leitura(arq, &filho_d);
            if (filho_d.nota > pai.nota) {
                printf("Erro: filho direito maior que pai na pos %d\n", i);
                return 0;
            }
        }
    }
    return 1;
}

void THEAP_constroi(char *nomeHeap, char *nomeDados){
    // FILE *arq_heap = fopen(nomeHeap, "rb+");
    // if (!arq_heap) {
    //     printf("Erro na criação. Arquivo da hash inválido.\n");
    //     return;
    // }
    FILE *arq_alunos = fopen(nomeDados, "rb+");
    if (!arq_alunos) {
        printf("Erro na criação. Arquivo dos nomes inválido.\n");
        return;
    }

    TA aluno;
    for(int i = 0; i < aluno_size; i++){
        TA_leitura(arq_alunos, &aluno);
        THEAP_insere(nomeHeap, aluno, i);
    }

    int a = THEAP_verifica(nomeHeap);
}
