//
// Created by Pichau on 11/07/2025.
//

#include "../hdr/THASH.h"

#include <stdlib.h>

#define TAM_THASH 100000
int THASH_h(long long int cpf, int k) {
    srand(cpf/100);
    return (rand()%TAM_THASH + k ) % TAM_THASH;
}

void THASH_escreve(char *nomeArq, TA *aluno) {
    FILE *hash = fopen(nomeArq, "rb+"); // abre para leitura e escrita sem apagar os dados
    if (!hash) {
        printf("[THASH_escreve]Erro ao abrir o arquivo '%s'!\n", nomeArq);
        exit(1);
    }

    srand(aluno->cpf/100);

    TA *registro = TA_inicializa();
    int indice = 0;
    int tentativas = 0;

    printf("[THASH_escreve]Buscando posicao para: nome:%s, cpf:%lld, nota:%d!\n", aluno->nome, aluno->cpf, aluno->nota);
    while (tentativas < TAM_THASH) {
        indice = THASH_h(aluno->cpf, tentativas);
        tentativas++;
        fseek(hash, (sizeof(TA) * indice), SEEK_SET);
        TA_leitura(hash, registro);

        if (registro->cpf == aluno->cpf) {
            printf("[THASH_escreve]Aluno ja esta na tabela!\n");
            fclose(hash);
            return;
        }

        if (registro->cpf == -1) {
            break;
        }
    }

    if (tentativas >= TAM_THASH) {
        printf("[THASH_escreve]Nao foi possivel escrever o registro, a hash esta cheia!(colisoes: %d)\n", tentativas);
        free(registro);
        fclose(hash);
        return;
    }

    fseek(hash, (sizeof(TA) * indice), SEEK_SET);
    TA_escrita(hash, aluno);
    printf("[THASH_escreve]Escrito com sucesso! (indice: %d, colisoes: %d)\n", indice, tentativas);
    free(registro);
    fclose(hash);
}

int THASH_busca(char *nomeArq, long long int cpf) {
    /* Retorna -1 caso não funcione ou não ache. Retorna o endereço para leitura do aluno caso ache.*/
    FILE *hash = fopen(nomeArq, "rb");
    if (!hash) {
        printf("[THASH_busca]Erro ao abrir o arquivo '%s'!\n", nomeArq);
        return -1;
    }

    srand(cpf/100);
    TA *registro = TA_inicializa();
    int indice = 0;
    int tentativas = 0;

    while (tentativas < TAM_THASH) {
        indice = THASH_h(cpf, tentativas);
        tentativas++;
        fseek(hash, (sizeof(TA) * indice), SEEK_SET);
        TA_leitura(hash, registro);

        if (registro->cpf == cpf) {
            printf("[THASH_busca]Encontramos o Aluno! (cpf: %lld)\n", cpf);
            free(registro);
            fclose(hash);
            return indice;
        }
    }
    fclose(hash);

    printf("[THASH_busca]Nao foi possivel encontrar o aluno!\n");
    free(registro);
    return -1;
}

void THASH_exclui(char *nomeArq, long long int cpf) {
    FILE *hash = fopen(nomeArq, "rb+");
    if (hash == NULL) {
        printf("[THASH_exclui]Nao foi possivel realizar a remoção!\n");
    } else{
        srand(cpf/100);
        TA *registro = TA_inicializa();
        int indice = -1;
        int tentativas = 0;
        int achou = 0;

        while (tentativas < TAM_THASH) {
            indice = THASH_h(cpf, tentativas);
            tentativas++;
            fseek(hash, (sizeof(TA) * indice), SEEK_SET);
            TA_leitura(hash, registro);

            if (registro->cpf == cpf) {
                printf("[THASH_exclui]Encontramos o Aluno! (cpf: %lld)\n", cpf);
                free(registro);
                achou = 1;
                break;
            }
        }
        if (achou) {
            TA *aluno = TA_inicializa();
            fseek(hash, (sizeof(TA) * indice), SEEK_SET);
            TA_escrita(hash, aluno);
        }else {
            printf("[THASH_exclui]Não foi possivel encontrar o elemento!\n");
        }
        fclose(hash);
    }
}

int THASH_leitura(char *nomeArq, long long int cpf, TA *aluno) {
    FILE *hash = fopen(nomeArq, "rb");
    if (hash == NULL) {
        printf("[THASH_leitura]Nao foi possivel realizar leitura!\n");
    } else {
        TA reg;
        reg.cpf = -1;
        int indice = 0;
        int tentativas = 0;
        int achou = 0;
        while (tentativas < TAM_THASH) {
            indice = THASH_h(cpf, tentativas++);
            fseek(hash, (sizeof(TA) * indice), SEEK_SET);
            // fread(&reg, sizeof(TA),1,arq);
            TA_leitura(hash, &reg);
            if (reg.cpf == cpf) {
                //printf("[THASH_busca]Encontramos o Aluno! (cpf: %lld)\n", cpf);
                achou = 1;
                break;
            }
        }
        if (achou) {
            fseek(hash, sizeof(TA) * indice, SEEK_SET);
            TA_leitura(hash, aluno);
        } else {
            //printf("[THASH_busca]Não foi possivel encontrar o elemento!\n");
            fclose(hash);
            return 0;
        }
        fclose(hash);
    }
    return 1;
}

void THASH_constroi(char *nomeHash, char *nomeDados) {
    FILE *hash = fopen(nomeHash, "wb+");
    if (hash == NULL) {
        printf("Erro ao abrir o %s!\n", nomeHash);
        exit(1);
    }

    FILE *dados = fopen(nomeDados, "rb");
    if (dados == NULL) {
        printf("Erro ao abrir o %s!\n", nomeDados);
        exit(1);
    }

    TA *aluno = TA_inicializa();
    for (int i = 0; i < TAM_THASH; i++) {
        TA_escrita(hash, aluno);
    }
    int offs = 0;
    while (TA_leitura(dados, aluno)) {
        int tentativas = 0;
        int indice = 0;
        TA reg;
        if (aluno->cpf == -1) {
            printf("aluno = -1\n");
        }
        while (tentativas < TAM_THASH) {
            indice = THASH_h(aluno->cpf, tentativas++);
            fseek(hash, indice * sizeof(TA), SEEK_SET);
            // fread(&reg, sizeof(TA),1,hash);
            TA_leitura(hash, &reg);
            if (reg.cpf == aluno->cpf) {
                printf("aluno ja na hash! (cpf:%lld)\n", reg.cpf);
                break;
            }
            if (reg.cpf == -1) {
                break;
            } // Posição livre
        }
        if (tentativas > TAM_THASH) {
            printf("nao consegui inserir: %d\n",offs++);
            break;
        }
        if (reg.cpf == aluno->cpf)continue;
        fseek(hash, indice * sizeof(TA), SEEK_SET);
        // fwrite(aluno, sizeof(TA),1,hash);
        TA_escrita(hash, aluno);
    }
    TA_libera(aluno);
    fclose(dados);
    fclose(hash);
}