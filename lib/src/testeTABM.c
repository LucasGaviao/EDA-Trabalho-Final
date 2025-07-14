// testeTABM.c (Refatorado e Completo)

#include "../hdr/testeTABM.h"
#include "../hdr/TA.h"
#include "../hdr/TABM.h"


// Função auxiliar para construir a árvore a partir de um arquivo de dados.
void TABM_constroi(char *nomeTABM, char *nomeTABMdados, char *nomeDados) {
    // 1. Inicializa (cria) o arquivo de índice da árvore
    TABM_inicializa(nomeTABM);

    // 2. Abre o arquivo de dados de origem (ex: 10k.bin)
    FILE *dados = fopen(nomeDados, "rb");
    if (!dados) {
        printf("Erro ao abrir o arquivo de dados de origem '%s'!\n", nomeDados);
        return;
    }

    // 3. Lê cada aluno do arquivo de origem e o insere na árvore
    TA aluno;
    printf("Construindo a Árvore B+... Isso pode levar um momento.\n");
    while (TA_leitura(dados, &aluno)) {
        TABM_insere(nomeTABM, nomeTABMdados, &aluno);
    }
    printf("Construção concluída!\n");
    fclose(dados);
}


// Função principal com o menu de teste
void testeTABM(char *nomeTABM, char *nomeTABMdados, char *nomeDados) {
    int escolha = 0;
    long long int endereco;
    TA *aluno = TA_inicializa();
    TA *aluno_tmp = TA_inicializa();

    while (escolha != 5) {
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        printf("Arquivos: - Índice Árvore B+ ('%s') - Dados Árvore B+ ('%s')\n", nomeTABM, nomeTABMdados);
        printf(" 1- Construir Árvore B+ a partir de '%s'.\n", nomeDados);
        printf(" 2- Inserir um Aluno.\n");
        printf(" 3- Remover um Aluno.\n");
        printf(" 4- Buscar um Aluno.\n");
        printf(" 5- Sair.\n");
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        printf("ESCOLHA: ");

        // Limpa o buffer de entrada para evitar loops infinitos com entradas inválidas
        if (scanf("%d", &escolha) != 1) {
            // Limpa o que quer que tenha sido digitado
            while (getchar() != '\n');
            escolha = 0; // Define uma escolha inválida para mostrar o menu novamente
        }

        switch (escolha) {
            case 1: // Construir Árvore B+
                printf("-=-=-=-=-=-=-=-=-= CONSTRUÇÃO =-=-=-=-=-=-=-=-=-\n");
                TABM_constroi(nomeTABM, nomeTABMdados, nomeDados);
                printf("[testeTABM] Árvore B+ construída com sucesso em '%s'.\n", nomeTABM);
                break;

            case 2: // Inserir Aluno
                printf("-=-=-=-=-=-=-=-=-=-= INSERÇÃO =-=-=-=-=-=-=-=-=-=-\n");
                printf("Digite o nome (até 49 caracteres): ");

                while (getchar() != '\n'); // Limpa o buffer do scanf anterior
                fgets(aluno->nome, 50, stdin);
                aluno->nome[strcspn(aluno->nome, "\n")] = 0; // Remove o \n

                printf("Insira um CPF de 11 dígitos: ");
                scanf("%lld", &aluno->cpf);

                printf("Insira uma nota (0 a 100): ");
                scanf("%d", &aluno->nota);

                printf("Inserindo o seguinte aluno:\n");
                TA_imprime(aluno);

                // CORREÇÃO: Passando o nome correto do arquivo de dados da árvore
                TABM_insere(nomeTABM, nomeTABMdados, aluno);

                // Verificação da inserção
                endereco = TABM_busca_cpf(nomeTABM, aluno->cpf);
                if (endereco != -1) {
                    printf("[testeTABM] Verificação: Aluno inserido e encontrado com sucesso!\n");
                } else {
                    printf("[testeTABM] Verificação: FALHA! Aluno não foi encontrado após a inserção!\n");
                }
                break;

            case 3: // Remover Aluno
                printf("-=-=-=-=-=-=-=-=-=-= REMOÇÃO =-=-=-=-=-=-=-=-=-=-\n");
                printf("Insira o CPF a ser removido: ");
                long long int cpf_removido;
                scanf("%lld", &cpf_removido);

                TABM_remove(nomeTABM, cpf_removido);

                // A verificação correta da remoção é tentar buscar novamente
                endereco = TABM_busca_cpf(nomeTABM, cpf_removido);
                if (endereco == -1) {
                    printf("[testeTABM] Verificação: Aluno removido com sucesso do índice!\n");
                } else {
                    printf("[testeTABM] Verificação: FALHA! Aluno ainda foi encontrado após a remoção!\n");
                }
                break;

            case 4: // Buscar Aluno
                printf("-=-=-=-=-=-=-=-=-=-=-= BUSCA =-=-=-=-=-=-=-=-=-=-=-\n");
                printf("Insira um CPF de 11 dígitos para buscar: ");
                long long int cpf_buscado;
                scanf("%lld", &cpf_buscado);

                endereco = TABM_busca_cpf(nomeTABM, cpf_buscado);

                if (endereco != -1) {
                    printf("[testeTABM] Aluno encontrado! Endereço no arquivo de dados: %lld\n", endereco);
                    FILE *dados_arvore = fopen(nomeTABMdados, "rb");
                    if (dados_arvore) {
                        // CORREÇÃO: fseek a partir do início do arquivo (SEEK_SET)
                        fseek(dados_arvore, endereco, SEEK_SET);
                        TA_leitura(dados_arvore, aluno_tmp);
                        printf("Dados do aluno encontrado:\n");
                        TA_imprime(aluno_tmp);
                        fclose(dados_arvore);
                    } else {
                        printf("[testeTABM] Erro: não foi possível abrir o arquivo de dados '%s' para ler o registro.\n", nomeTABMdados);
                    }
                } else {
                    printf("[testeTABM] Aluno com CPF %lld não encontrado na árvore.\n", cpf_buscado);
                }
                break;

            case 5: // Sair
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                printf("Saindo do programa de teste da Árvore B+.\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }

    // Libera a memória alocada no início
    TA_libera(aluno);
    TA_libera(aluno_tmp);
}