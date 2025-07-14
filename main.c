#include <stdio.h>

#include "lib/hdr/THASH.h"
#include "lib/hdr/gerar10k.h"
#include "lib/hdr/testehash.h"

#define  TAM_MAIN 10000 // deve ser >= valor em gerar_10k.c(TAM_10K)

int main(void) {
    // long long int all[TAM_MAIN];
    // gerar_10k("10k.bin",all);
    // THASH_constroi("tabela.bin","10k.bin");
    // TA a;
    // int achados = 0;
    // for (int i = 0; i < TAM_MAIN; i++) {
        // achados += THASH_leitura("tabela.bin", all[i], &a);
    // }
    // printf("achamos: %d\n",achados);
    teste_hash("tabela.bin", "10k.bin");
    // teste_ler_10k("tabela.bin","10k.bin");
    return 0;
}