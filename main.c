#include <stdio.h>

#include "lib/hdr/testehash.h"
#include "lib/hdr/testeHeap.h"

#define  TAM_MAIN 10000 // deve ser >= valor em gerar_10k.c(TAM_10K)

int main(void) {
    int flag = 1;
    int escolha;
    while (flag) {
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= MENU -=-=-=--=-=-=-=-=-=-=-=-=-=-=-\n");
        printf(" 1- HASH\n 2- Heap\n");
        printf("ESCOLHA: ");
        scanf("%d", &escolha);
        if (escolha == 1) {
            teste_hash("tabela.bin", "10k.bin");
        }else if (escolha == 2) {
            testeHeap("heap.bin", "10k.bin");
        } else {
            exit(0);
        }

    }

    return 0;
}