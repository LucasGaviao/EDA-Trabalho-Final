// TABM.c

#include "../hdr/TABM.h"
static void TABM_insere_recursivo(TABM* no, TA* aluno, const char* arq_idx, const char* arq_dados, Cabecalho* cab);
static void TABM_divisao(TABM* pai, int i_filho, TABM* filho, const char* arq_idx, Cabecalho* cab);
static TABM TABM_criar_no(Cabecalho* cab);

// --- Funções Auxiliares de I/O ---

void ler_cabecalho(FILE* arq, Cabecalho* cab) {
    fseek(arq, 0, SEEK_SET);
    fread(cab, sizeof(Cabecalho), 1, arq);
}

void escrever_cabecalho(FILE* arq, Cabecalho* cab) {
    fseek(arq, 0, SEEK_SET);
    fwrite(cab, sizeof(Cabecalho), 1, arq);
}

void TABM_ler_no(const char* arq_idx, long long endereco, TABM* no) {
    FILE* f = fopen(arq_idx, "rb");
    if (!f) return;
    fseek(f, endereco, SEEK_SET);
    fread(no, sizeof(TABM), 1, f);
    fclose(f);
}

void TABM_escrever_no(const char* arq_idx, TABM* no) {
    FILE* f = fopen(arq_idx, "rb+");
    if (!f) return;
    fseek(f, no->endereco, SEEK_SET);
    fwrite(no, sizeof(TABM), 1, f);
    fclose(f);
}

// --- Funções Principais ---

void TABM_inicializa(const char* arq_idx_nome) {
    FILE* arq_idx = fopen(arq_idx_nome, "wb");
    if (!arq_idx) {
        perror("Não foi possível criar o arquivo de índice.");
        exit(1);
    }
    Cabecalho cab;
    cab.endereco_raiz = -1;
    cab.proximo_endereco_livre = sizeof(Cabecalho); // O primeiro nó será escrito após o cabeçalho
    escrever_cabecalho(arq_idx, &cab);
    fclose(arq_idx);
}

TABM TABM_criar_no(Cabecalho* cab) {
    TABM no;
    memset(&no, 0, sizeof(TABM));
    no.eh_folha = 1;
    no.proximo_no = -1;
    no.num_chaves = 0;
    no.endereco = cab->proximo_endereco_livre;
    cab->proximo_endereco_livre += sizeof(TABM);
    return no;
}

void TABM_insere(const char* arq_idx_nome, const char* arq_dados_nome, TA* aluno) {
    FILE* arq_idx = fopen(arq_idx_nome, "rb+");
    if (!arq_idx) {
        perror("Arquivo de índice não encontrado. Use a função de inicialização primeiro.");
        return;
    }

    Cabecalho cab;
    ler_cabecalho(arq_idx, &cab);
    fclose(arq_idx);

    if (TABM_busca_cpf(arq_idx_nome, aluno->cpf) != -1) {
        printf("CPF %lld já existe na árvore.\n", aluno->cpf);
        return;
    }

    TABM raiz;
    if (cab.endereco_raiz == -1) {
        raiz = TABM_criar_no(&cab);
        cab.endereco_raiz = raiz.endereco;
        TABM_escrever_no(arq_idx_nome, &raiz);
    } else {
        TABM_ler_no(arq_idx_nome, cab.endereco_raiz, &raiz);
    }

    if (raiz.num_chaves == (2 * TAM_TABM - 1)) {
        TABM nova_raiz = TABM_criar_no(&cab);
        nova_raiz.eh_folha = 0;
        nova_raiz.filhos[0] = raiz.endereco;

        TABM_divisao(&nova_raiz, 0, &raiz, arq_idx_nome, &cab);
        TABM_insere_recursivo(&nova_raiz, aluno, arq_idx_nome, arq_dados_nome, &cab);

        cab.endereco_raiz = nova_raiz.endereco;
    } else {
        TABM_insere_recursivo(&raiz, aluno, arq_idx_nome, arq_dados_nome, &cab);
    }

    // Reescreve o cabeçalho com os dados atualizados (nova raiz ou novo endereço livre)
    arq_idx = fopen(arq_idx_nome, "rb+");
    escrever_cabecalho(arq_idx, &cab);
    fclose(arq_idx);
}

void TABM_insere_recursivo(TABM* no, TA* aluno, const char* arq_idx, const char* arq_dados, Cabecalho* cab) {
    if (no->eh_folha) {
        int i = no->num_chaves - 1;
        // Compara com long long int
        while (i >= 0 && aluno->cpf < no->chaves[i]) {
            no->chaves[i + 1] = no->chaves[i]; // Atribuição direta
            no->reg[i + 1] = no->reg[i];
            i--;
        }
        no->chaves[i + 1] = aluno->cpf; // Atribuição direta
        no->num_chaves++;

        FILE* f_dados = fopen(arq_dados, "ab"); // Append binary
        if (f_dados) {
            fseek(f_dados, 0, SEEK_END);
            no->reg[i + 1] = ftell(f_dados); // Guarda o endereço do registro de dados
            TA_escrita(f_dados, aluno); // Usa a função de escrita de TA
            fclose(f_dados);
        }
        TABM_escrever_no(arq_idx, no);
    } else {
        int i = no->num_chaves - 1;
        while (i >= 0 && aluno->cpf < no->chaves[i]) {
            i--;
        }
        i++;

        TABM filho;
        TABM_ler_no(arq_idx, no->filhos[i], &filho);

        if (filho.num_chaves == (2 * TAM_TABM - 1)) {
            TABM_divisao(no, i, &filho, arq_idx, cab);
            if (aluno->cpf > no->chaves[i]) {
                i++;
            }
        }
        TABM_ler_no(arq_idx, no->filhos[i], &filho);
        TABM_insere_recursivo(&filho, aluno, arq_idx, arq_dados, cab);
    }
}

void TABM_divisao(TABM* pai, int i_filho, TABM* filho, const char* arq_idx, Cabecalho* cab) {
    TABM novo_irmao = TABM_criar_no(cab);
    novo_irmao.eh_folha = filho->eh_folha;

    // Move a metade direita das chaves do nó cheio (filho) para o novo nó (novo_irmao)
    if (filho->eh_folha) {
        novo_irmao.num_chaves = TAM_TABM; // A nova folha recebe uma chave a mais
        filho->num_chaves = TAM_TABM - 1;

        for (int j = 0; j < TAM_TABM; j++) {
            novo_irmao.chaves[j] = filho->chaves[j + TAM_TABM - 1];
            novo_irmao.reg[j] = filho->reg[j + TAM_TABM - 1];
        }

        // Ajusta a lista encadeada de folhas
        novo_irmao.proximo_no = filho->proximo_no;
        filho->proximo_no = novo_irmao.endereco;

    } else { // Se for um nó interno
        novo_irmao.num_chaves = TAM_TABM - 1;
        filho->num_chaves = TAM_TABM - 1;

        for (int j = 0; j < TAM_TABM - 1; j++) {
            novo_irmao.chaves[j] = filho->chaves[j + TAM_TABM];
        }
        for (int j = 0; j < TAM_TABM; j++) {
            novo_irmao.filhos[j] = filho->filhos[j + TAM_TABM];
        }
    }

    // Desloca os filhos do pai para abrir espaço para o novo nó
    for (int j = pai->num_chaves; j > i_filho; j--) {
        pai->filhos[j + 1] = pai->filhos[j];
    }
    pai->filhos[i_filho + 1] = novo_irmao.endereco;

    // Desloca as chaves do pai para abrir espaço para a nova chave
    for (int j = pai->num_chaves - 1; j >= i_filho; j--) {
        pai->chaves[j + 1] = pai->chaves[j];
    }

    // Promove a chave correta para o pai
    if (filho->eh_folha) {
        // Para folhas, a primeira chave do novo irmão é COPIADA para o pai [cite: 95, 133]
        pai->chaves[i_filho] = novo_irmao.chaves[0];
    } else {
        // Para nós internos, a chave do meio SOBE e é removida do filho [cite: 99]
        pai->chaves[i_filho] = filho->chaves[TAM_TABM - 1];
    }

    pai->num_chaves++;

    // Salva todas as alterações no disco
    TABM_escrever_no(arq_idx, pai);
    TABM_escrever_no(arq_idx, filho);
    TABM_escrever_no(arq_idx, &novo_irmao);
}

long long TABM_busca_cpf(const char* arq_idx_nome, long long int cpf) {
    FILE* arq_idx = fopen(arq_idx_nome, "rb");
    if (!arq_idx) return -1;

    Cabecalho cab;
    ler_cabecalho(arq_idx, &cab);
    fclose(arq_idx);

    if (cab.endereco_raiz == -1) return -1;

    TABM no_atual;
    TABM_ler_no(arq_idx_nome, cab.endereco_raiz, &no_atual);

    // Navega pela árvore até chegar em um nó folha
    while (!no_atual.eh_folha) {
        int i = 0;
        // Encontra o primeiro filho cujo divisor é MAIOR que a chave
        while (i < no_atual.num_chaves && cpf >= no_atual.chaves[i]) {
            i++;
        }
        // Desce para o filho encontrado [cite: 67, 68]
        TABM_ler_no(arq_idx_nome, no_atual.filhos[i], &no_atual);
    }

    // Na folha, faz uma busca linear pela chave
    for (int i = 0; i < no_atual.num_chaves; i++) {
        if (cpf == no_atual.chaves[i]) {
            return no_atual.reg[i]; // Retorna o endereço do registro de dados
        }
    }

    return -1; // Não encontrado
}

// Função auxiliar para encontrar o nó folha para remoção (o seu já estava bom)
long long encontrar_folha_para_remocao(const char* arq_idx_nome, long long endereco_no, long long int cpf) {
    TABM no;
    TABM_ler_no(arq_idx_nome, endereco_no, &no);
    while (!no.eh_folha) {
        int i = 0;
        while (i < no.num_chaves && cpf >= no.chaves[i]) {
            i++;
        }
        TABM_ler_no(arq_idx_nome, no.filhos[i], &no);
    }
    return no.endereco;
}

// Função auxiliar para remover a chave de um nó folha
void remover_chave_de_folha(TABM* folha, long long int cpf) {
    int i = 0;
    while (i < folha->num_chaves && cpf != folha->chaves[i]) {
        i++;
    }
    if (i == folha->num_chaves) return; // Chave não está na folha

    // Desloca as chaves e registros para preencher o espaço
    for (int j = i; j < folha->num_chaves - 1; j++) {
        folha->chaves[j] = folha->chaves[j + 1];
        folha->reg[j] = folha->reg[j + 1];
    }
    folha->num_chaves--;
}


void TABM_remove(const char* arq_idx_nome, long long int cpf) {
    // A implementação completa da remoção com redistribuição e concatenação
    // em todos os níveis é extremamente complexa.
    // Esta versão simplificada foca na remoção da folha.
    // O rebalanceamento completo não está implementado.

    long long endereco_dado = TABM_busca_cpf(arq_idx_nome, cpf);
    if (endereco_dado == -1) {
        printf("Erro: CPF %lld não encontrado para remoção.\n", cpf);
        return;
    }

    FILE* arq_idx = fopen(arq_idx_nome, "rb+");
    if (!arq_idx) return;

    Cabecalho cab;
    ler_cabecalho(arq_idx, &cab);
    fclose(arq_idx);

    long long endereco_folha = encontrar_folha_para_remocao(arq_idx_nome, cab.endereco_raiz, cpf);

    TABM folha;
    TABM_ler_no(arq_idx_nome, endereco_folha, &folha);

    // Remove a chave e o ponteiro do registro da folha [cite: 364]
    remover_chave_de_folha(&folha, cpf);

    TABM_escrever_no(arq_idx_nome, &folha);

    printf("Chave %lld removida da folha. A chave pode permanecer nos nós internos como guia. [cite: 365]\n", cpf);
    printf("Aviso: O rebalanceamento completo (concatenação/redistribuição) não está implementado.\n");
}