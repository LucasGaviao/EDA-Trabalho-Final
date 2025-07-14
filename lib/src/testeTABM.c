// testeTABM.c (Corrigido e Funcional)

#include "../hdr/testeTABM.h"
#include "../hdr/TA.h"
#include "../hdr/TABM.h"

// Função principal com o menu de teste
void testeTABM(char *nomeTABM, char *nomeTABMdados, char *nomeDados) {
    long long raiz = constroi_arvore(nomeDados, nomeTABM, nomeTABMdados);
    if (raiz == -1) {
        printf("Falha critica na construcao da arvore. Encerrando.\n");
        return;
    }
    int flag = 1;
    int opcao;
    while (flag) {
        printf("\n====================================\n");
        printf("   MENU INTERATIVO - ARVORE B+\n");
        printf("====================================\n");
        printf("1. Inserir um novo aluno\n");
        printf("2. Remover um aluno por CPF\n");
        printf("3. Buscar um aluno por CPF\n");
        printf("0. Sair\n");
        printf("------------------------------------\n");
        printf("ESCOLHA: ");

        scanf("%d", &opcao);
        long long int cpf_temp;
        if (opcao == 1) {
                printf("\n--- Inserir Novo Aluno ---\n");
                TA *novo_aluno = TA_inicializa();

                printf("Nome Completo: ");
                int c;
                while ((c = getchar()) != '\n' && c != EOF);

                char nomeTemp[100];
                fgets(nomeTemp, sizeof(nomeTemp), stdin);
                for (int i = 0; i < 49; i++) {
                    if (nomeTemp[i] == '\n') {
                        nomeTemp[i] = ' ';
                    }
                }
                memset(novo_aluno->nome, 0, 50);
                strncpy(novo_aluno->nome, nomeTemp, 49);
                novo_aluno->nome[49] = '\0';

                printf("CPF (11 digitos): ");
                scanf("%lld", &novo_aluno->cpf);

                printf("Nota Final: ");
                scanf("%d", &novo_aluno->nota);

                raiz = TARVBP_insere(raiz, novo_aluno, nomeTABM, nomeTABMdados);
                printf("Tentativa de insercao finalizada. Nova raiz: %lld\n", raiz);
                free(novo_aluno);
        }else if (opcao == 2) {
            printf("\n--- Remover Aluno ---\n");
            printf("CPF (11 digitos): ");
            scanf("%lld", &cpf_temp);

            raiz = TARVBP_remove(raiz, cpf_temp, nomeTABM);
        } else if (opcao == 3) {
            printf("\n--- Buscar Aluno ---\n");
            printf("CPF (11 digitos): ");
            scanf("%lld", &cpf_temp);

            buscar_e_imprimir_dados(raiz, cpf_temp, nomeTABM, nomeDados);
        }else {
            flag = 0;
            return;
        }
    }
    return;
}