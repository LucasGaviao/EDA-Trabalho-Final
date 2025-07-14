//
// Created by lucas on 14/07/2025.
//

#include "../hdr/testeHeap.h"


void testeHeap(char *nomeHeap, char *nomeDados) {
    printf("-=-=-=-=-=-=-=-=-= MENU =-=-=-=-=-=-=-=-=-\nArquivos: - Heap('%s') - Dados('%s')\n 1- Construir Heap.\n 2- Inserir Aluno.\n 3- Remover Aluno.\n 4- Buscar Aluno.\n OUTRO- Sair\n", nomeHeap, nomeDados);
    FILE *heap_temp = fopen(nomeHeap, "rb");
    long int valor = -1;
    long int *tam_heap = &valor;
    if (heap_temp == NULL) {
        *tam_heap = 0;
    }else {
        fseek(heap_temp, 0, SEEK_END);
        *tam_heap = ftell(heap_temp)/sizeof(TA);
    }
    fclose(heap_temp);

    int opcao = 1;
    TA *aluno = TA_inicializa();
    TA *aluno2 = TA_inicializa();
    int indice = 0, x = 10, imprimir = 0;
    while ((1 <= opcao) && (opcao < 6)) {
        printf("ESCOLHA: ");
        scanf("%d", &opcao);
        if (opcao == 1) {
            printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
            THEAP_constroi(nomeHeap, nomeDados, tam_heap);
            printf("[testeHeap.case2] Heap construida em '%s'!\n", nomeHeap);
        } else if (opcao == 2) {
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

            printf("[testeHeap.case2]Insira um CPF de 11 digitos: ");
            scanf("%lld", &aluno->cpf);

            printf("[testeHeap.case2]Insira uma nota (0 a 100): ");
            scanf("%d", &aluno->nota);

            TA_imprime(aluno);
            THEAP_escreve(nomeHeap, aluno, tam_heap);
        }else if (opcao == 3) {
            printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
            THEAP_exclui(nomeHeap, tam_heap);
        }else if (opcao == 4) {
            printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
            printf("[testeHeap.case4]Insira um CPF de 11 digitos: ");
            scanf("%lld", &aluno->cpf);

            free(aluno2);
            aluno2 = THEAP_busca(nomeHeap, aluno->cpf, tam_heap);
            if (!aluno2) {
                printf("[testeHeap.case4] Busca falhou!\n");
            } else {
                printf("[testeHeap.case4] Busca Realizada!\n");
                TA_imprime(aluno2);
            }
        } else {
            printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
            printf("[testeHeap.case6] Saindo de [testeHeap]\n");
            return;
        }
        printf("-=-=-=-=-=-=-=-=-= MENU =-=-=-=-=-=-=-=-=-\nArquivos: - Heap('%s') - Dados('%s')\n 1- Construir Heap.\n 2- Inserir Aluno.\n 3- Remover Aluno.\n 4- Buscar Aluno.\n OUTRO- Sair\n", nomeHeap, nomeDados);
    }

}
