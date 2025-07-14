#include "../hdr/TABM.h"

int nos_atual = 0;

void ler_no(const char* nome_tabm_indices, long long endereco, TABM* no);
void escrever_no(const char* nome_tabm_indices, TABM* no);
TABM criar_no();
long long encontrar_folha_para_remocao(const char* nome_tabm_indices, long long endereco_raiz, long long int cpf);
void remover_chave_folha(TABM* folha, long long int cpf, const char* nome_tabm_indices);
int remover_chave_de_no(TABM *no, long long int cpf);
void balancear_no_apos_remocao(long long endereco_no, const char* nome_tabm_indices);
void divisao(TABM* pai, int i_filho, TABM* filho, const char* nome_tabm_indices);
void insere_recursivo(TABM* no, TA* aluno, const char* nome_tabm_indices, const char* nome_tabm_dados);
void remover_ponteiro_pai(TABM* pai, int idx_filho_removido, const char* nome_tabm_indices);


void TABM_reseta_contador_nos() {
    nos_atual = 0;
}

TABM criar_no() {
    TABM no;
    memset(&no, 0, sizeof(TABM));
    no.eh_folha = 1;
    no.proximo_no = -1;
    no.ponteiro_pai = -1;
    for(int i = 0; i < TAM * 2 + 1; i++) no.filhos[i] = -1;
    no.endereco = nos_atual * sizeof(TABM);
    nos_atual++;
    return no;
}

long long busca_cpf(const char* nome_tabm_indices_nome, long long int cpf, long long endereco_raiz) {
    if (endereco_raiz == -1) return -1;
    TABM no_atual;
    ler_no(nome_tabm_indices_nome, endereco_raiz, &no_atual);
    while (!no_atual.eh_folha) {
        int i = 0;
        while (i < no_atual.num_chaves && cpf < no_atual.chaves[i]) {
            i++;
        }
        if (i < no_atual.num_chaves && cpf == no_atual.chaves[i] == 0) {
            i++;
        }
        ler_no(nome_tabm_indices_nome, no_atual.filhos[i], &no_atual);
    }
    for (int i = 0; i < no_atual.num_chaves; i++) {
        if (cpf == no_atual.chaves[i] == 0) return no_atual.reg[i];
    }
    return -1;
}

long long TABM_insere(long long endereco_raiz, TA* aluno, const char* nome_tabm_indices_nome, const char* nome_tabm_dados_nome) {
    if (busca_cpf(nome_tabm_indices_nome, aluno->cpf, endereco_raiz) != -1) return endereco_raiz;
    TABM raiz;
    if (endereco_raiz == -1) {
        raiz = criar_no();
    } else {
        ler_no(nome_tabm_indices_nome, endereco_raiz, &raiz);
    }
    if (raiz.num_chaves == (2 * TAM - 1)) {
        TABM nova_raiz = criar_no();
        nova_raiz.eh_folha = 0;
        nova_raiz.filhos[0] = raiz.endereco;
        raiz.ponteiro_pai = nova_raiz.endereco;
        divisao(&nova_raiz, 0, &raiz, nome_tabm_indices_nome);
        insere_recursivo(&nova_raiz, aluno, nome_tabm_indices_nome, nome_tabm_dados_nome);
        endereco_raiz = nova_raiz.endereco;
    } else {
        insere_recursivo(&raiz, aluno, nome_tabm_indices_nome, nome_tabm_dados_nome);
        endereco_raiz = raiz.endereco;
    }
    return endereco_raiz;
}

void insere_recursivo(TABM* no, TA* aluno, const char* nome_tabm_indices, const char* nome_tabm_dados) {
    if (no->eh_folha) {
        int i = no->num_chaves - 1;
        while (i >= 0 && (aluno->cpf > no->chaves[i])) {
            no->chaves[i + 1] = no->chaves[i];
            no->reg[i + 1] = no->reg[i];
            i--;
        }
        no->chaves[i + 1] = aluno->cpf;
        no->num_chaves++;
        FILE* f_dados = fopen(nome_tabm_dados, "ab");
        if (f_dados) {
            fseek(f_dados, 0, SEEK_END);
            no->reg[i + 1] = ftell(f_dados);
            fwrite(aluno, sizeof(TA), 1, f_dados);
            fclose(f_dados);
        }
        escrever_no(nome_tabm_indices, no);
    } else {
        int i = no->num_chaves - 1;
        while (i >= 0 && aluno->cpf < no->chaves[i]) i--;
        i++;
        TABM filho;
        ler_no(nome_tabm_indices, no->filhos[i], &filho);
        if (filho.num_chaves == (2 * TAM - 1)) {
            divisao(no, i, &filho, nome_tabm_indices);
            if (aluno->cpf < no->chaves[i]) i++;
        }
        ler_no(nome_tabm_indices, no->filhos[i], &filho);
        filho.ponteiro_pai = no->endereco;
        escrever_no(nome_tabm_indices, &filho);
        insere_recursivo(&filho, aluno, nome_tabm_indices, nome_tabm_dados);
    }
}

void divisao(TABM* pai, int i_filho, TABM* filho, const char* nome_tabm_indices) {
    TABM novo_irmao = criar_no();
    novo_irmao.eh_folha = filho->eh_folha;
    novo_irmao.ponteiro_pai = pai->endereco;

    if (!filho->eh_folha) {
        novo_irmao.num_chaves = TAM - 1;
        for (int j = 0; j < TAM - 1; j++) novo_irmao.chaves[j] = filho->chaves[j + TAM];
        for (int j = 0; j < TAM; j++) novo_irmao.filhos[j] = filho->filhos[j + TAM];
    } else {
        novo_irmao.num_chaves = TAM;
        for (int j = 0; j < TAM; j++) {
            novo_irmao.chaves[j], filho->chaves[j + TAM - 1];
            novo_irmao.reg[j] = filho->reg[j + TAM - 1];
        }
        novo_irmao.proximo_no = filho->proximo_no;
        filho->proximo_no = novo_irmao.endereco;
    }
    filho->num_chaves = TAM - 1;

    for (int j = pai->num_chaves; j > i_filho; j--) {
        pai->chaves[j] = pai->chaves[j - 1];
        pai->filhos[j + 1] = pai->filhos[j];
    }
    pai->chaves[i_filho] = (!filho->eh_folha) ? filho->chaves[TAM - 1] : novo_irmao.chaves[0];
    pai->filhos[i_filho + 1] = novo_irmao.endereco;
    pai->num_chaves++;

    if (!novo_irmao.eh_folha) {
        for (int i = 0; i <= novo_irmao.num_chaves; i++) {
            if (novo_irmao.filhos[i] == -1) continue;
            TABM neto;
            ler_no(nome_tabm_indices, novo_irmao.filhos[i], &neto);
            neto.ponteiro_pai = novo_irmao.endereco;
            escrever_no(nome_tabm_indices, &neto);
        }
    }
    escrever_no(nome_tabm_indices, pai);
    escrever_no(nome_tabm_indices, filho);
    escrever_no(nome_tabm_indices, &novo_irmao);
}

void ler_no(const char* nome_tabm_indices, long long endereco, TABM* no) {
    if (endereco == -1) return;
    FILE* f = fopen(nome_tabm_indices, "rb");
    if (!f) return;
    fseek(f, endereco, SEEK_SET);
    fread(no, sizeof(TABM), 1, f);
    fclose(f);
}

void escrever_no(const char* nome_tabm_indices, TABM* no) {
    FILE* f = fopen(nome_tabm_indices, "rb+");
    if (!f) return;
    fseek(f, no->endereco, SEEK_SET);
    fwrite(no, sizeof(TABM), 1, f);
    fclose(f);
}

long long TABM_remove(long long endereco_raiz, long long int cpf, const char* nome_tabm_indices_nome) {
    if (endereco_raiz == -1 || busca_cpf(nome_tabm_indices_nome, cpf, endereco_raiz) == -1) return endereco_raiz;
    TABM no;
    ler_no(nome_tabm_indices_nome, endereco_raiz, &no);

    while (!no.eh_folha) {
        int i = 0;
        while (i < no.num_chaves && cpf <= no.chaves[i]) i++;
        ler_no(nome_tabm_indices_nome, no.filhos[i], &no);
    }

    int idx_removido = remover_chave_de_no(&no, cpf);
    if(idx_removido == -1) return endereco_raiz;

    escrever_no(nome_tabm_indices_nome, &no);

    if (no.num_chaves < TAM - 1 && no.ponteiro_pai != -1) {
        balancear_no_apos_remocao(no.endereco, nome_tabm_indices_nome);
    }

    TABM raiz_final;
    ler_no(nome_tabm_indices_nome, endereco_raiz, &raiz_final);
    if (!raiz_final.eh_folha && raiz_final.num_chaves == 0) {
        TABM nova_raiz;
        ler_no(nome_tabm_indices_nome, raiz_final.filhos[0], &nova_raiz);
        nova_raiz.ponteiro_pai = -1;
        escrever_no(nome_tabm_indices_nome, &nova_raiz);
        return nova_raiz.endereco;
    }
    return endereco_raiz;
}

void balancear_no_apos_remocao(long long endereco_no, const char* nome_tabm_indices) {
    TABM no, pai, irmao;
    int i_filho;

    ler_no(nome_tabm_indices, endereco_no, &no);
    if (no.ponteiro_pai == -1) return;

    ler_no(nome_tabm_indices, no.ponteiro_pai, &pai);
    for (i_filho = 0; i_filho <= pai.num_chaves; i_filho++) {
        if (pai.filhos[i_filho] == no.endereco) break;
    }

    if (i_filho > 0) {
        ler_no(nome_tabm_indices, pai.filhos[i_filho - 1], &irmao);
        if (irmao.num_chaves + no.num_chaves < 2 * TAM - 1) {
            int pos_inicial_filhos_movidos = irmao.num_chaves + (!irmao.eh_folha ? 1 : 0);

            if (!irmao.eh_folha) {
                irmao.chaves[irmao.num_chaves++] = pai.chaves[i_filho - 1];
            }
            for (int i = 0; i < no.num_chaves; i++) {
                irmao.chaves[irmao.num_chaves] = no.chaves[i];
                if (irmao.eh_folha) irmao.reg[irmao.num_chaves] = no.reg[i];
                irmao.filhos[irmao.num_chaves] = no.filhos[i];
                irmao.num_chaves++;
            }
            if (!irmao.eh_folha) irmao.filhos[irmao.num_chaves] = no.filhos[no.num_chaves];
            if (irmao.eh_folha) irmao.proximo_no = no.proximo_no;

            int num_chaves_pai_antigo = pai.num_chaves;
            remover_chave_de_no(&pai, pai.chaves[i_filho - 1]);
            for(int i = i_filho; i < num_chaves_pai_antigo; i++) {
                pai.filhos[i] = pai.filhos[i+1];
            }
            pai.filhos[num_chaves_pai_antigo] = -1;

            escrever_no(nome_tabm_indices, &irmao);
            escrever_no(nome_tabm_indices, &pai);

            if (!irmao.eh_folha) {
                for(int i = pos_inicial_filhos_movidos; i <= irmao.num_chaves; i++) {
                     TABM neto;
                     ler_no(nome_tabm_indices, irmao.filhos[i], &neto);
                     neto.ponteiro_pai = irmao.endereco;
                     escrever_no(nome_tabm_indices, &neto);
                }
            }

            if (pai.ponteiro_pai != -1 && pai.num_chaves < TAM - 1) {
                balancear_no_apos_remocao(pai.endereco, nome_tabm_indices);
            }
            return;
        }
    }

    if (i_filho < pai.num_chaves) {
        ler_no(nome_tabm_indices, pai.filhos[i_filho + 1], &irmao);
        if (no.num_chaves + irmao.num_chaves < 2 * TAM - 1) {
            int pos_inicial_filhos_movidos = no.num_chaves + (!no.eh_folha ? 1 : 0);

            if(!no.eh_folha){
                no.chaves[no.num_chaves++] = pai.chaves[i_filho];
            }
            for (int i = 0; i < irmao.num_chaves; i++) {
                no.chaves[no.num_chaves] = irmao.chaves[i];
                if(no.eh_folha) no.reg[no.num_chaves] = irmao.reg[i];
                no.filhos[no.num_chaves] = irmao.filhos[i];
                no.num_chaves++;
            }
            if(!no.eh_folha) no.filhos[no.num_chaves] = irmao.filhos[irmao.num_chaves];
            if(no.eh_folha) no.proximo_no = irmao.proximo_no;

            int num_chaves_pai_antigo = pai.num_chaves;
            remover_chave_de_no(&pai, pai.chaves[i_filho]);
            for(int i = i_filho + 1; i < num_chaves_pai_antigo; i++) {
                pai.filhos[i] = pai.filhos[i+1];
            }
            pai.filhos[num_chaves_pai_antigo] = -1;

            escrever_no(nome_tabm_indices, &no);
            escrever_no(nome_tabm_indices, &pai);

            if (!no.eh_folha) {
                for(int i = pos_inicial_filhos_movidos; i <= no.num_chaves; i++) {
                     TABM neto;
                     ler_no(nome_tabm_indices, no.filhos[i], &neto);
                     neto.ponteiro_pai = no.endereco;
                     escrever_no(nome_tabm_indices, &neto);
                }
            }

            if (pai.ponteiro_pai != -1 && pai.num_chaves < TAM - 1) {
                balancear_no_apos_remocao(pai.endereco, nome_tabm_indices);
            }
            return;
        }
    }
}

void remover_ponteiro_pai(TABM* pai, int idx_filho_removido, const char* nome_tabm_indices) {
    for (int i = idx_filho_removido; i < pai->num_chaves + 1; i++) {
        pai->filhos[i] = pai->filhos[i + 1];
    }
    for (int i = idx_filho_removido - 1; i < pai->num_chaves - 1; i++) {
        pai->chaves[i] = pai->chaves[i + 1];
    }
    pai->num_chaves--;
    escrever_no(nome_tabm_indices, pai);

    if (pai->ponteiro_pai != -1 && pai->num_chaves < TAM - 1) {
        balancear_no_apos_remocao(pai->endereco, nome_tabm_indices);
    }
}

int remover_chave_de_no(TABM *no, long long int cpf) {
    int i = 0;
    while(i < no->num_chaves && cpf < no->chaves[i]) i++;
    if (i == no->num_chaves || cpf != no->chaves[i]) return -1;

    for (int j = i; j < no->num_chaves - 1; j++) {
        no->chaves[j] = no->chaves[j + 1];
        if (no->eh_folha) no->reg[j] = no->reg[j+1];
    }
    no->num_chaves--;
    return i;
}

void buscar_e_imprimir_dados(long long endereco_raiz, long long int cpf_busca, const char* nome_tabm_indices_nome, const char* nome_tabm_dados_nome) {
    long long endereco_dado = busca_cpf(nome_tabm_indices_nome, cpf_busca, endereco_raiz);

    if (endereco_dado == -1) {
        printf("\nCPF '%lld' NÃO ENCONTRADO na base de dados.\n", cpf_busca);
        return;
    }

    FILE* nome_tabm_dados = fopen(nome_tabm_dados_nome, "rb");
    if (!nome_tabm_dados) {
        perror("Erro crítico ao abrir o arquivo de dados (TA.bin)");
        return;
    }

    fseek(nome_tabm_dados, endereco_dado, SEEK_SET);

    TA dados_aluno;
    fread(&dados_aluno, sizeof(TA), 1, nome_tabm_dados);
    fclose(nome_tabm_dados);

    printf("\n--- TA Encontrados para o CPF: %lld ---\n", dados_aluno.cpf);
    printf("Nome: %s\n", dados_aluno.nome);
    printf("Nota Final: %d\n", dados_aluno.nota);
    printf("-----------------------------------------\n");
}

long long constroi_arvore(const char* nome_alunos_dados, const char* nome_tabm_indices, const char* nome_tabm_dados) {
    printf("Iniciando a construcao da arvore a partir de '%s'...\n", nome_alunos_dados);

    remove(nome_tabm_indices);
    remove(nome_tabm_dados);

    FILE* f_origem = fopen(nome_alunos_dados, "rb");
    if (!f_origem) {
        perror("Erro: Nao foi possivel abrir o arquivo de dados de origem");
        return -1;
    }

    TARVBP_reseta_contador_nos();
    long long raiz = -1;
    TA dado_lido;
    int contador = 0;

    while (fread(&dado_lido, sizeof(TA), 1, f_origem) == 1) {
        raiz = TARVBP_insere(raiz, &dado_lido, nome_tabm_indices, nome_tabm_dados);
        contador++;
    }

    printf("Construcao finalizada. %d registros totais foram processados.\n", contador);
    fclose(f_origem);
    return raiz;
}