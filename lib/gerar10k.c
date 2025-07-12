//
// Created by Pichau on 11/07/2025.
//

#include "gerar10k.h"

int gerar_nota() {
    return abs(rand()) % 101;
}

long long int gerar_cpf () {
    return abs((rand() * 100 + 10000000000 + rand()/2 + rand()*200 + rand()*5000)) % 99999999999;
}

void atualiza_nome(char *nome, int num) {
    for (int i = 0; i <= 5 && nome[i] != '\0'; i++) {
        if (num > 0) {
            int j = num%10;
            num = num/10;
            char k = '0' + j;
            nome[48-i] = k;
        }
    }
    nome[50] = '\0';
}

int gerar_10k() {
    printf("[gerar_10k]Iniciando operacao!\n");
    srand((unsigned long)time(NULL));
    char nome[] = "aluno00000000000000000000000000000000000000000000\0";
    TA *aluno = TA_inicializa();

    FILE *arq = fopen("10kaluno.bin", "wb+");
    if (!arq) {
        printf("[gerar_10k]Nao foi possivel abrir o arquivo!\n");
        fclose(arq);
        return 0;
    }
    printf("[gerar_10k]Foi possivel abrir o arquivo!\n");

    for (int i = 0; i < 10000; i++) {
        atualiza_nome(nome, i);
        TA_set_nome(aluno, nome);
        TA_set_cpf(aluno, gerar_cpf());
        TA_set_nota(aluno, gerar_nota());
        TA_escrita(arq, aluno);
    }

    TA_libera(aluno);
    fclose(arq);
    printf("[gerar_10k]Operacao finalizada!\n");
    return 1;
}