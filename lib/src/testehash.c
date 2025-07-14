//
// Created by Pichau on 11/07/2025.
//

#include "../hdr/testehash.h"
#define K10 10000

int teste_ler_X_alunos(char *nomeHash, char *nomeDados, int x, int imprimir) {
    FILE *alunos = fopen(nomeDados, "rb");
    if (alunos == NULL) {
        printf("[teste_ler_10k]Erro ao abrir %s! \n", nomeDados);
        exit(1);
    }
    int contador = 0;
    for (int i = 0; i < x && !feof(alunos); i++) {
        TA aluno_f;
        TA aluno_h;
        if (TA_leitura(alunos, &aluno_f) != 1) {
            printf("[teste_ler_10k]Erro ao realizar a leitura em %s!\n", nomeDados);
            return -1;
        }
        if (aluno_f.cpf < 0) {
            printf("[teste_ler_10k]Erro! Dado corrompido!\n");
            return -1;
        }

        contador += THASH_leitura(nomeHash, aluno_f.cpf, &aluno_h);
        if (imprimir) {
            TA_imprime(&aluno_h);
        }
    }
    if (contador >= x) {
        printf("[teste_ler_10k]Todas as operacoes foram realizadas! %d.\n", contador);
        return 1;
    }
    printf("[teste_ler_10k]Operacoes corrompidas! %d.\n",x-contador);
    return 0;
}

void teste_hash(char *nomeHash, char *nomeDados) {
    printf("-=-=-=-=-=-=-=-=-= MENU =-=-=-=-=-=-=-=-=-\nArquivos: - Hash('%s') - Dados('%s')\n 1- Construir Hash.\n 2- Inserir Aluno.\n 3- Remover Aluno.\n 4- Buscar Aluno.\n 5- Ler X registros.\n 6- Sair.\n", nomeHash, nomeDados);
    int opcao = -1;
    TA *aluno = TA_inicializa();
    int indice = 0, x = 10, imprimir = 0;
    while (opcao != 6) {
        printf("ESCOLHA: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                THASH_constroi(nomeHash, nomeDados);
                break;
            case 2:
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                printf("[testeHash.case2]Digite o nome (até 49 caracteres): ");

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

                printf("[testeHash.case2]Insira um CPF de 11 dígitos: ");
                scanf("%lld", &aluno->cpf);

                printf("[testeHash.case2]Insira uma nota (0 a 100): ");
                scanf("%d", &aluno->nota);

                TA_imprime(aluno);
                THASH_escreve(nomeHash, aluno);
                break;
            case 3:
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                printf("[testeHash.case3]Insira um CPF de 11 dígitos: ");
                scanf("%lld", &aluno->cpf);

                indice = THASH_busca(nomeHash, aluno->cpf);
                THASH_exclui(nomeHash, aluno->cpf);
                FILE *hash = fopen(nomeDados, "rb");
                if (hash == NULL) { printf("[testeHash.case3]Erro ao abrir o arquivo!\n"); exit(1);}
                fseek(hash, sizeof(TA)*indice, SEEK_END);
                TA_leitura(hash, aluno);
                TA_imprime(aluno);
                if (aluno->cpf < 0) {
                    printf("[testeHash.case3] Aluno removido com sucesso!\n");
                    TA_imprime(aluno);
                }
                fclose(hash);
                break;
            case 4:
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                printf("[testeHash.case4]Insira um CPF de 11 dígitos: ");
                scanf("%lld", &aluno->cpf);
                indice = THASH_busca(nomeHash, aluno->cpf);
                if (indice < 0) {
                    printf("[testeHash.case4] Busca falhou!\n");
                } else {
                    printf("[testeHash.case4] Busca Realizada! O aluno esta no indice %d.\n", indice);
                }
                break;
            case 5:
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                printf("Insira a quantidade de elementos: ");
                scanf("%d", &x);
                printf("Insira 1 para impressao e 0 caso contrario([1]impacta na performance): ");
                scanf("%d", &imprimir);
                teste_ler_X_alunos(nomeHash, nomeDados, x, imprimir);
                break;
            case 6:
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                printf("[testeHash.case6] Saindo de [testeHash]\n");
                return;
        }
        printf("-=-=-=-=-=-=-=-=-= MENU =-=-=-=-=-=-=-=-=-\nArquivos: - Hash('%s') - Dados('%s')\n 1- Construir Hash.\n 2- Inserir Aluno.\n 3- Remover Aluno.\n 4- Buscar Aluno.\n 5- Ler X registros.\n 6- Sair.\n", nomeHash, nomeDados);
    }

}