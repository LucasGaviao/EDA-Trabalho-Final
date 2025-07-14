//
// Created by lucas on 14/07/2025.
//

#include "../hdr/testeTABM.h"


void TABM_constroi(char *nomeTABM, char *nomeTABMdados, char *nomeDados) {
    TABM_inicializa(nomeTABM);
    FILE *dados = fopen(nomeDados, "rb");
    if (!dados) {
        printf("Erro ao abrir '%s'!\n", nomeDados);
        return;
    }
    TA aluno;
    while (TA_leitura(dados, &aluno)) {
        TABM_insere(nomeTABM, nomeTABMdados, &aluno);
    }
    fclose(dados);
}

void testeTABM(char *nomeTABM, char *nomeTABMdados, char *nomeDados) {
    int flag = 1;
    int escolha;
    long long int indice;
    TA *aluno_tmp = TA_inicializa();
    TA *aluno = TA_inicializa();
    while (flag) {
        printf("-=-=-=-=-=-=-=-=-= MENU =-=-=-=-=-=-=-=-=-\nArquivos: - TABM('%s') - Dados('%s')\n 1- Construir Heap.\n 2- Inserir Aluno.\n 3- Remover Aluno.\n 4- Buscar Aluno.\n OUTRO- Sair\n", nomeTABM, nomeDados);
        printf("ESCOLHA: ");
        scanf("%d", &escolha);
        if (escolha == 1) {
            printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
            TABM_constroi(nomeTABM, nomeTABMdados,nomeDados);
            printf("[testeTABM.case1] Arquivo '%s' construido!\n", nomeTABM);
        } else if (escolha == 2) {
            printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
            printf("[testeHeap.case2]Digite o nome (ate 49 caracteres): ");

            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            char nomeTemp[100];
            fgets(nomeTemp, sizeof(nomeTemp), stdin);
            for (int i = 0; i < 49; i++) {
                if (nomeTemp[i] == '\n') {
                    nomeTemp[i] = ' ';
                }
            }
            memset(aluno->nome, 0, 50);
            strncpy(aluno->nome, nomeTemp, 49);
            aluno->nome[49] = '\0';

            printf("[testeTABM.case2]Insira um CPF de 11 digitos: ");
            scanf("%lld", &aluno->cpf);

            printf("[testeTABM.case2]Insira uma nota (0 a 100): ");
            scanf("%d", &aluno->nota);

            TA_imprime(aluno);
            TABM_insere(nomeTABM, nomeTemp, aluno);
            indice = TABM_busca_cpf(nomeTABM, aluno->cpf);
            if (indice != -1) {
                FILE *TABM_dados = fopen(nomeTABMdados, "rb");
                if (!TABM_dados) {printf("[testeTABM.case2]Erro ao abrir aquivo de dados da arvore!\n"); return;}
                fseek(TABM_dados, indice * sizeof(TA), SEEK_END);
                TA_leitura(TABM_dados, aluno_tmp);
                if (aluno->cpf == aluno_tmp->cpf) printf("[testeTABM.case2] Aluno inserido com sucesso!\n");
                else printf("[testeTABM.case2] Falha ao inserir o aluno!\n");
                printf("[testeTABM.case2] Aluno encontrado no indice!\n");
                TA_imprime(aluno);
            }
        } else {
            printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
            printf("[testeTABM.caseOUTROS] Saindo de [testeTABM]\n");
            return;
        }
    }
}
