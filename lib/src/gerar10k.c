//
// Created by Pichau on 11/07/2025.
//

#include "../hdr/gerar10k.h"
#define TAM_10K 10000

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

int gerar_10k(char *nomeDados) {
    printf("[gerar_10k]Iniciando operacao!\n");
    srand((unsigned long)time(NULL));
    char nome[] = "aluno00000000000000000000000000000000000000000000\0";
    TA *aluno = TA_inicializa();
    int index = 0;
    FILE *arq = fopen(nomeDados, "wb");
    if (!arq) {
        printf("[gerar_10k]Nao foi possivel abrir o %s!\n", nomeDados);
        fclose(arq);
        return 0;
    }
    printf("[gerar_10k]Foi possivel abrir o arquivo!\n");

    for (int i = 0; i < TAM_10K; i++) {
        atualiza_nome(nome, i);
        memset(aluno,0x00,sizeof(TA));
        TA_set_nome(aluno, nome);
        TA_set_cpf(aluno, gerar_cpf());
        TA_set_nota(aluno, gerar_nota());
        if (TA_escrita(arq, aluno) != 1) exit(1);
    }
    TA_libera(aluno);
    fclose(arq);
    printf("[gerar_10k]Operacao finalizada!\n");
    return 1;
}

int ler_10k(char *nomeDados) {
    FILE *dados = fopen(nomeDados, "rb");
    if (!dados) {
        printf("[ler_10k]Nao foi possivel abrir o %s!\n", nomeDados);
        fclose(dados);
        return 0;
    }
    TA *aluno = TA_inicializa();
    for (int i = 0; i < TAM_10K; i++) {
        if (TA_leitura(dados, aluno) != 1) {
            printf("[ler_10k] FALHOU!\n");
            exit(1);
        }
        // TA_imprime(aluno);
    }

    free(aluno);
    fclose(dados);
    printf("[ler_10k]Concluido com sucesso!\n");
    return 1;
}

void gerar_menu(char *nomeDados) {
    int flag = 1;
    int escolha;
    while (flag) {
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= MENU -=-=-=--=-=-=-=-=-=-=-=-=-=-=-\n Arquivo: '%s'\n", nomeDados);
        printf(" 1- Gerar '%s'.\n Outro- Sair\n", nomeDados);
        printf("ESCOLHA: ");
        scanf("%d", &escolha);
        if (escolha == 1) {
            gerar_10k(nomeDados);
            printf("[gerar_menu.case1] Arquivo '%s' gerado!\n", nomeDados);
        } else {
            flag = 0;
            return;
        }
    }
}