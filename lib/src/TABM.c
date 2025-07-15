#include "../hdr/TABM.h"

int numero_nos = 0;

void reseta_numero_nos() {
    numero_nos = 0;
}

TABM TABM_criar_no() {
    TABM no;
    memset(&no, 0, sizeof(TABM));
    no.eh_folha = 1;
    no.proximo_no = -1;
    no.ponteiro_pai = -1;
    for(int i = 0; i < d * 2 + 1; i++) no.filhos[i] = -1;
    no.endereco = numero_nos * sizeof(TABM);
    numero_nos++;
    return no;
}

long long int TABM_escreve(long long int endereco_raiz, TA aluno, const char* nome_tabm_indices, const char* nome_tabm_dados) {
    if (TABM_busca_cpf(nome_tabm_indices, aluno.cpf, endereco_raiz) != -1) return endereco_raiz;

    TABM root;
    if (endereco_raiz == -1) {
        root = TABM_criar_no();
    } else {
        TABM_leitura_no(nome_tabm_indices, endereco_raiz, &root);
    }
    endereco_raiz = root.endereco;
    if (root.num_chaves == (2 * d - 1)) {
        TABM new_root = TABM_criar_no();
        new_root.eh_folha = 0;
        new_root.filhos[0] = root.endereco;
        root.ponteiro_pai = new_root.endereco;
        TABM_divide(&new_root, 0, &root, nome_tabm_indices);
        TABM_escreve_recursivo(&new_root, aluno, nome_tabm_indices, nome_tabm_dados);
        endereco_raiz = new_root.endereco;
    } else {
        TABM_escreve_recursivo(&root, aluno, nome_tabm_indices, nome_tabm_dados);
    }
    return endereco_raiz;
}

void TABM_escreve_recursivo(TABM* no, TA aluno, const char* nome_tabm_indices, const char* nome_tabm_dados) {
    if (no->eh_folha) {
        int i = no->num_chaves - 1;
        while (i >= 0 && aluno.cpf < no->chaves[i]) {
            no->chaves[i + 1] = no->chaves[i];
            no->registros[i + 1] = no->registros[i];
            i--;
        }
        no->chaves[i + 1] = aluno.cpf;
        no->num_chaves++;

        FILE* student_fp = fopen(nome_tabm_dados, "ab");
        if (student_fp) {
            fseek(student_fp, 0, SEEK_END);
            no->registros[i + 1] = ftell(student_fp);
            fwrite(&aluno, sizeof(TA), 1, student_fp);
            fclose(student_fp);
        }
        TABM_escreve_no(nome_tabm_indices, *no);
    } else {
        int i = no->num_chaves - 1;
        while (i >= 0 && aluno.cpf < no->chaves[i] < 0) i--;
        i++;

        TABM child;
        TABM_leitura_no(nome_tabm_indices, no->filhos[i], &child);

        if (child.num_chaves == (2 * d - 1)) {
            TABM_divide(no, i, &child, nome_tabm_indices);
            if (aluno.cpf > no->chaves[i]) i++;
        }

        TABM correct_c;

        TABM_leitura_no(nome_tabm_indices, no->filhos[i], &correct_c);
        TABM_escreve_recursivo(&correct_c, aluno, nome_tabm_indices, nome_tabm_dados);
    }
}

void TABM_divide(TABM* pai, int indice_filho, TABM* filho, const char* nome_tabm_indices) {
    TABM novo_no = TABM_criar_no();
    novo_no.eh_folha = filho->eh_folha;
    novo_no.ponteiro_pai = pai->endereco;

    if (!filho->eh_folha) {
        novo_no.num_chaves = d - 1;
        for (int j = 0; j < d - 1; j++)
            novo_no.chaves[j] = filho->chaves[j + d];
        for (int j = 0; j < d; j++)
            novo_no.filhos[j] = filho->filhos[j + d];
    } else {
        novo_no.num_chaves = d;
        for (int j = 0; j < d; j++) {
            novo_no.chaves[j] = filho->chaves[j + d - 1];
            novo_no.registros[j] = filho->registros[j + d - 1];
        }
        novo_no.proximo_no = filho->proximo_no;
        filho->proximo_no = novo_no.endereco;
    }
    filho->num_chaves = d - 1;

    for (int j = pai->num_chaves; j > indice_filho; j--) {
        pai->chaves[j] = pai->chaves[j - 1];
        pai->filhos[j + 1] = pai->filhos[j];
    }
    pai->chaves[indice_filho] = (!filho->eh_folha) ? filho->chaves[d - 1] : novo_no.chaves[0];
    pai->filhos[indice_filho + 1] = novo_no.endereco;
    pai->num_chaves++;

    if (!novo_no.eh_folha) {
        for (int i = 0; i <= novo_no.num_chaves; i++) {
            if (novo_no.filhos[i] == -1) continue;
            TABM no_temp;
            TABM_leitura_no(nome_tabm_indices, novo_no.filhos[i], &no_temp);
            no_temp.ponteiro_pai = novo_no.endereco;
            TABM_escreve_no(nome_tabm_indices, no_temp);
        }
    }
    TABM_escreve_no(nome_tabm_indices, *pai);
    TABM_escreve_no(nome_tabm_indices, *filho);
    TABM_escreve_no(nome_tabm_indices, novo_no);
}

void TABM_leitura_no(const char* nome_tabm_indices, long long int endereco, TABM* no){
    if (endereco == -1) return;
    FILE* fp = fopen(nome_tabm_indices, "rb");
    if (!fp) return;
    fseek(fp, endereco, SEEK_SET);
    fread(no, sizeof(TABM), 1, fp);
    fclose(fp);
}

void TABM_escreve_no(const char* nome_tabm_indices, TABM no) {
    FILE* fp = fopen(nome_tabm_indices, "rb+");
    if (!fp) return;
    fseek(fp, no.endereco, SEEK_SET);
    fwrite(&no, sizeof(TABM), 1, fp);
    fclose(fp);
}

long long int TABM_busca_cpf(const char* nome_tabm_indices, long long int cpf, long long int endereco_raiz) {
    if (endereco_raiz == -1) return -1;

    TABM atual;
    TABM_leitura_no(nome_tabm_indices, endereco_raiz, &atual);

    while (!atual.eh_folha) {
        int i = 0;
        while (i < atual.num_chaves && cpf > atual.chaves[i]) {
            i++;
        }
        if (i < atual.num_chaves && cpf == atual.chaves[i]) {
            i++;
        }
        TABM_leitura_no(nome_tabm_indices, atual.filhos[i], &atual);
    }

    for (int i = 0; i < atual.num_chaves; i++) {
        if (cpf == atual.chaves[i])
            return atual.registros[i];
    }
    return -1;
}

long long int TABM_remove(long long endereco_raiz, long long int cpf, const char* nome_tabm_indices) {
    if (endereco_raiz == -1 || TABM_busca_cpf(nome_tabm_indices, cpf, endereco_raiz) == -1)
        return endereco_raiz;

    TABM atual;
    TABM_leitura_no(nome_tabm_indices, endereco_raiz, &atual);

    while (!atual.eh_folha) {
        int i = 0;
        while (i < atual.num_chaves && cpf >= atual.chaves[i]) i++;
        TABM_leitura_no(nome_tabm_indices, atual.filhos[i], &atual);
    }

    int endereco_removido = TABM_remove_chave_no(&atual, cpf);
    if(endereco_removido == -1) return endereco_raiz;

    TABM_escreve_no(nome_tabm_indices, atual);

    if (atual.num_chaves < d - 1 && atual.ponteiro_pai != -1) {
        TABM_balanceia(atual.endereco, nome_tabm_indices);
    }

    TABM raiz;
    TABM_leitura_no(nome_tabm_indices, endereco_raiz, &raiz);
    if (!raiz.eh_folha && raiz.num_chaves == 0) {
        TABM nova_raiz;
        TABM_leitura_no(nome_tabm_indices, raiz.filhos[0], &nova_raiz);
        nova_raiz.ponteiro_pai = -1;
        TABM_escreve_no(nome_tabm_indices, nova_raiz);
        return nova_raiz.endereco;
    }
    return endereco_raiz;
}

void TABM_balanceia(long long endereco_no, const char* nome_tabm_indices) {
    TABM no, pai, irmao;
    int endereco_filho;

    TABM_leitura_no(nome_tabm_indices, endereco_no, &no);
    if (no.ponteiro_pai == -1) return;

    TABM_leitura_no(nome_tabm_indices, no.ponteiro_pai, &pai);
    for (endereco_filho = 0; endereco_filho <= pai.num_chaves; endereco_filho++) {
        if (pai.filhos[endereco_filho] == no.endereco) break;
    }

    if (endereco_filho > 0) {
        TABM_leitura_no(nome_tabm_indices, pai.filhos[endereco_filho - 1], &irmao);
        if (irmao.num_chaves + no.num_chaves < 2 * d - 1) {
            int endereco_inicial = irmao.num_chaves + (!irmao.eh_folha ? 1 : 0);

            if (!irmao.eh_folha) {
                irmao.chaves[irmao.num_chaves++] = pai.chaves[endereco_filho - 1];
            }
            for (int i = 0; i < no.num_chaves; i++) {
                irmao.chaves[irmao.num_chaves] = no.chaves[i];
                if (irmao.eh_folha) irmao.registros[irmao.num_chaves] = no.registros[i];
                irmao.filhos[irmao.num_chaves] = no.filhos[i];
                irmao.num_chaves++;
            }
            if (!irmao.eh_folha) irmao.filhos[irmao.num_chaves] = no.filhos[no.num_chaves];
            if (irmao.eh_folha) irmao.proximo_no = no.proximo_no;

            int ex_numero_chaves = pai.num_chaves;
            TABM_remove_chave_no(&pai, pai.chaves[endereco_filho - 1]);
            for(int i = endereco_filho; i < ex_numero_chaves; i++) {
                pai.filhos[i] = pai.filhos[i+1];
            }
            pai.filhos[ex_numero_chaves] = -1;

            TABM_escreve_no(nome_tabm_indices, irmao);
            TABM_escreve_no(nome_tabm_indices, pai);

            if (!irmao.eh_folha) {
                for(int i = endereco_inicial; i <= irmao.num_chaves; i++) {
                    TABM filho;
                    TABM_leitura_no(nome_tabm_indices, irmao.filhos[i], &filho);
                    filho.ponteiro_pai = irmao.endereco;
                    TABM_escreve_no(nome_tabm_indices, filho);
                }
            }

            if (pai.ponteiro_pai != -1 && pai.num_chaves < d - 1) {
                TABM_balanceia(pai.endereco, nome_tabm_indices);
            }
            return;
        }
    }

    if (endereco_filho < pai.num_chaves) {
        TABM_leitura_no(nome_tabm_indices, pai.filhos[endereco_filho + 1], &irmao);
        if (no.num_chaves + irmao.num_chaves < 2 * d - 1) {
            int endereco_inicial = no.num_chaves + (!no.eh_folha ? 1 : 0);

            if(!no.eh_folha){
                no.chaves[no.num_chaves++] = pai.chaves[endereco_filho];
            }
            for (int i = 0; i < irmao.num_chaves; i++) {
                no.chaves[no.num_chaves] = irmao.chaves[i];
                if(no.eh_folha) no.registros[no.num_chaves] = irmao.registros[i];
                no.filhos[no.num_chaves] = irmao.filhos[i];
                no.num_chaves++;
            }
            if(!no.eh_folha) no.filhos[no.num_chaves] = irmao.filhos[irmao.num_chaves];
            if(no.eh_folha) no.proximo_no = irmao.proximo_no;

            int ex_numero_chaves = pai.num_chaves;
            TABM_remove_chave_no(&pai, pai.chaves[endereco_filho]);
            for(int i = endereco_filho + 1; i < ex_numero_chaves; i++) {
                pai.filhos[i] = pai.filhos[i+1];
            }
            pai.filhos[ex_numero_chaves] = -1;

            TABM_escreve_no(nome_tabm_indices, no);
            TABM_escreve_no(nome_tabm_indices, pai);

            if (!no.eh_folha) {
                for(int i = endereco_inicial; i <= no.num_chaves; i++) {
                    TABM filho_2;
                    TABM_leitura_no(nome_tabm_indices, no.filhos[i], &filho_2);
                    filho_2.ponteiro_pai = no.endereco;
                    TABM_escreve_no(nome_tabm_indices, filho_2);
                }
            }

            if (pai.ponteiro_pai != -1 && pai.num_chaves < d - 1) {
                TABM_balanceia(pai.endereco, nome_tabm_indices);
            }
            return;
        }
    }
}

void TABM_remove_ponteiro_pai(TABM pai, int endereco_filho_removido, const char* nome_tabm_indices) {
    for (int i = endereco_filho_removido; i < pai.num_chaves + 1; i++) {
        pai.filhos[i] = pai.filhos[i + 1];
    }
    for (int i = endereco_filho_removido - 1; i < pai.num_chaves - 1; i++) {
        pai.chaves[i] = pai.chaves[i + 1];
    }
    pai.num_chaves--;
    TABM_escreve_no(nome_tabm_indices, pai);

    if (pai.ponteiro_pai != -1 && pai.num_chaves < d - 1) {
        TABM_balanceia(pai.endereco, nome_tabm_indices);
    }
}

int TABM_remove_chave_no(TABM* no, long long int cpf) {
    int i = 0;
    while(i < no->num_chaves && cpf > no->chaves[i]) i++;
    if (i == no->num_chaves || cpf != no->chaves[i]) return -1;

    for (int j = i; j < no->num_chaves - 1; j++) {
        no->chaves[j] = no->chaves[j + 1];
        if (no->eh_folha) no->registros[j] = no->registros[j+1];
    }
    no->num_chaves--;
    return i;
}

void TABM_busca_e_imprime(long long endereco_raiz, long long int cpf_alvo, const char* nome_tabm_indices, const char* nome_tabm_dados) {
    long long endereco_dados = TABM_busca_cpf(nome_tabm_indices, cpf_alvo, endereco_raiz);

    if (endereco_dados == -1) {
        printf("\nCPF '%s' NOT FOUND in database.\n", cpf_alvo);
        return;
    }

    FILE* tabm_dados = fopen(nome_tabm_dados, "rb");
    if (!tabm_dados) {
        perror("Critical error opening data file");
        return;
    }

    fseek(tabm_dados, endereco_dados, SEEK_SET);

    TA aluno_tabm;
    fread(&aluno_tabm, sizeof(TA), 1, tabm_dados);
    fclose(tabm_dados);

    printf("\n--- Data Found for CPF: %lld ---\n", aluno_tabm.cpf);
    printf("Name: %s\n", aluno_tabm.nome);
    printf("Final Score: %d\n", aluno_tabm.nota);
    printf("--------------------------------\n");
}

void TABM_constroi(FILE *nome_alunos_dados){

    printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
    printf("Construindo a árvore:\n");

    rewind(nome_alunos_dados);

    TA alunos = {0};
    long long vet[10000];
    long long int endereco_raiz = -1;
    int j = 0;

    for(int i = 0; i < 10000; i++){
        if(fread(&alunos, sizeof(TA), 1, nome_alunos_dados) != 1) break;
        vet[i] = alunos.cpf;
        j++;
        endereco_raiz = TABM_escreve(endereco_raiz, alunos, "index_btree.bin", "leaf_btree.bin");
        printf("Posição da raíz: %lld\nInserimos o cpf: %lld\nTentativa: %d\n", endereco_raiz, alunos.cpf, i);
        printf("\n-----------------------n\n");
    }


    printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
    printf("Fazendo uma busca de todos os elementos para verificar inserção:\n");

    int completas = 0;
    for (int i = 0; i < j; i++) {
            long long int result = TABM_busca_cpf("leaf_btree.bin", vet[i], endereco_raiz);
            if (result != -1) {
                //printf("%d: found: %lld\n", i + 1, result);
                completas++;
            }
            else printf("cpf não encontrado: %lld.\nInteração: %d.\n", vet[i], i + 1);
        }
    printf("Achamos %d cpfs.\nNúmero total de cpfs: %d.\n", completas, j);

    printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
    printf("Removendo 100 elementos aleatórios:\n");

    long long int cpfs_to_remove[100];


    for(int i = 0; i < 100; i++){
        srand(i + j);
        int random_position = rand() % j;
        cpfs_to_remove[i] = vet[random_position];
        printf("Removendo: %lld.\n", vet[random_position]);
        endereco_raiz = TABM_remove(endereco_raiz, vet[random_position], "index_btree.bin");
        printf("Removido com sucesso. Nova raíz: %lld\n", vet[random_position], endereco_raiz);
        vet[random_position] = -1;
    }

    printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
    printf("Fazendo a verificação se os elementos foram removidos corretamente:\n");

    completas = 0;
    int nao_achou = 0;

    for (int i = 0; i < j; i++) {
        if (TABM_busca_cpf("index_btree.bin", vet[i], endereco_raiz) != -1){
            completas++;
        } else {
            if (vet[i] != -1)printf("O cpf %lld não foi encontrado, ma deveria existir.\n", vet[i]);
        }
    }

    for (int i = 0; i < 100; i++) {
        if (TABM_busca_cpf("index_btree.bin", cpfs_to_remove[i], endereco_raiz) == -1) {
            nao_achou++;
        } else {
             printf("O cpf %lld foi encontrado, mas não deveria existir.\n", vet[i]);
        }
    }

    printf("\n--- Resumo ---\n");
    printf("Estudantes encontrados: %d/%d.\n", completas, j - 100);
    printf("Estudantes não encontrados(Removidos): %d/100\n", nao_achou);

}