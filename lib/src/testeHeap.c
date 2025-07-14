//
// Created by lucas on 14/07/2025.
//

#include "../hdr/testeHeap.h"


void testeHeap(char *nomeHeap, char *nomeDados) {
    printf("-=-=-=-=-=-=-=-=-= MENU =-=-=-=-=-=-=-=-=-\nArquivos: - Heap('%s') - Dados('%s')\n 1- Construir Heap.\n 2- Inserir Aluno.\n 3- Remover Aluno.\n 4- Buscar Aluno.\n 5- Verifica estrutura\n OUTRO- Sair\n", nomeHeap, nomeDados);
    long int valor = -1;
    long int *tam_heap = &valor;

    if (THEAP_retorna_tam(tam_heap)){
        printf("[testeHeap] Tamanho atual da heap: %ld.\n", *tam_heap);
    }else {
        printf("[testeHeap] Heap ainda não existe. Dica: Crie uma na opção [1]!\n");
    }

    int opcao = 1;
    TA *aluno = TA_inicializa();
    TA *aluno2 = TA_inicializa();
    while ((1 <= opcao) && (opcao < 6)) {
        printf("Tamanho atual: %ld\n", *tam_heap);
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
        } else if (opcao == 5) {
            if (THEAP_verifica(nomeHeap)) printf("[testeHeap.case5] Estrutura correta!\n");
            else printf("[testeHeap.case5] Heap corrompida!\n");
        } else {
            printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
            printf("[testeHeap.case6] Saindo de [testeHeap]\n");
            THEAP_guarda_tam(tam_heap);
            return;
        }
        printf("-=-=-=-=-=-=-=-=-= MENU =-=-=-=-=-=-=-=-=-\nArquivos: - Heap('%s') - Dados('%s')\n 1- Construir Heap.\n 2- Inserir Aluno.\n 3- Remover Aluno.\n 4- Buscar Aluno.\n 5- Verifica estrutura\n OUTRO- Sair\n", nomeHeap, nomeDados);
    }

}
