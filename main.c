#include <stdio.h>

#include "lib/hdr/gerar10k.h"
#include "lib/hdr/testehash.h"
#include "lib/hdr/testeHeap.h"
#include "lib/hdr/testeTABM.h"

#define  TAM_MAIN 10000 // deve ser >= valor em gerar_10k.c(TAM_10K)

int main(void) {
    int flag = 1;
    int escolha;
    while (flag) {
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= MENU -=-=-=--=-=-=-=-=-=-=-=-=-=-=-\n");
        printf(" 1- Alunos\n 2- Hash\n 3- Heap\n 4- TABM\n OUTRO- Sair\n");
        printf("ESCOLHA: ");
        scanf("%d", &escolha);
        if (escolha == 2) {
            teste_hash("tabela.bin", "10k.bin");
        }else if (escolha == 3) {
            testeHeap("heap.bin", "10k.bin");
        } else if (escolha == 1) {
            gerar_menu("10k.bin");
        } else if (escolha == 4) {
            testeTABM("tabm_idx.bin", "tabm_dados.bin", "10k.bin");
        } else {
            exit(0);
        }

    }

    return 0;
}