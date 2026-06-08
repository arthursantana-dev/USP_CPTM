#include "CRUD.h"

int busca_sequencial_e_delete(FILE *arquivo_dados, FILE *fab, Header *header, Estacao *estacao_busca)
{
    char buffer[TAM_REGISTRO];

    // bool removeu_estacao = false;

    header_arvore_b header_b = arvore_b_ler_cabecalho(fab);

    int RRNnovo = 0;
    int RRN_atual = -1;

    Estacao *estacao = (Estacao *)calloc(1, sizeof(Estacao));

    while (fread(buffer, TAM_REGISTRO, 1, arquivo_dados) == 1)
    {
        printf("%d\n", RRN_atual);

        escrever_buffer_na_estacao(buffer, estacao);

        RRN_atual++;

        if (estacao->removido == '1')
        {
            limpar_estacao(estacao);
            continue;
        }

        if (!comparar_estacoes(estacao_busca, estacao))
        {
            limpar_estacao(estacao);
            continue;
        }

        // removeu_estacao = true;

        RRNnovo = RRN_atual;

        estacao->removido = '1';
        estacao->proximo = header->topo;

        header->topo = RRNnovo;

        escrever_estacao_no_buffer(estacao, buffer);

        arvore_b_remover(fab, &header_b, estacao->codEstacao);

        fseek(arquivo_dados, TAM_HEADER + TAM_REGISTRO * RRNnovo, SEEK_SET);

        escrever_buffer_no_arquivo(arquivo_dados, buffer);

        limpar_estacao(estacao);

        fseek(arquivo_dados, 0, SEEK_CUR);
    }

    destruir_estacao(estacao);

    return EXIT_SUCCESS;
}

int busca_em_indice_e_delete(FILE *arquivo_dados, FILE *fab, Header *header, Estacao *estacao_busca)
{
    header_arvore_b header_b = arvore_b_ler_cabecalho(fab);

    int byte_offset = arvore_b_buscar(fab, &header_b, estacao_busca->codEstacao);

    // Se retornou -1, o registro não existe, então a deleção está concluída
    if (byte_offset == -1)
    {
        return EXIT_SUCCESS;
    }

    char buffer[TAM_REGISTRO];
    Estacao *estacao = (Estacao *)calloc(1, sizeof(Estacao));

    fseek(arquivo_dados, byte_offset, SEEK_SET);
    if (fread(buffer, TAM_REGISTRO, 1, arquivo_dados) == 1)
    {
        escrever_buffer_na_estacao(buffer, estacao);

        // Verifica se já não foi removido e se bate com o restante da busca
        if (estacao->removido == '0' && comparar_estacoes(estacao_busca, estacao))
        {
            // Calcula qual é o RRN para empilhar no topo do cabeçalho
            int rrn_atual = (byte_offset - TAM_HEADER) / TAM_REGISTRO;

            // Atualiza a estação para marcação lógica (dados)
            estacao->removido = '1';
            estacao->proximo = header->topo;
            header->topo = rrn_atual;

            // Reescreve no buffer e salva no disco na mesma posição
            escrever_estacao_no_buffer(estacao, buffer);
            fseek(arquivo_dados, byte_offset, SEEK_SET);
            escrever_buffer_no_arquivo(arquivo_dados, buffer);

            // Remove a chave do índice (índice)
            arvore_b_remover(fab, &header_b, estacao_busca->codEstacao);
        }

        limpar_estacao(estacao);
    }

    destruir_estacao(estacao);

    return EXIT_SUCCESS;
}

int crud_delete(Estacao *estacao_busca, FILE *f, FILE *fab, Header *header)
{

    // int removeu_estacao = 0;
    int erro;

    if (estacao_busca->codEstacao != -1 && estacao_busca->codEstacao != 0)
    {
        erro = busca_em_indice_e_delete(f, fab, header, estacao_busca);
    }
    else
    {
        erro = busca_sequencial_e_delete(f, fab, header, estacao_busca);
    }
    if(erro){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int DELETE(int n, FILE *f, FILE *fab)
{

    if (f == NULL)
    {
        mostrar_erro();
        return EXIT_FAILURE;
    }

    int err = 0;

    Estacao *estacao = criar_estacao_para_busca(0, "", 0, "", 0, 0, 0, 0);

    Header *header = ler_header_do_arquivo(f);

    if (header == NULL)
    {
        fclose(f);
        mostrar_erro();
        return EXIT_FAILURE;
    }

    header->status = '0';
    escrever_header_no_arquivo(f, header);

    for (int i = 0; i < n; i++)
    {

        set_valores_estacao_para_busca(estacao);

        ler_input_para_estacao_de_busca(estacao);

        printf("Estação a ser buscada: ");
        imprimir_estacao(estacao);

        err = crud_delete(estacao, f, fab, header);

        limpar_estacao(estacao);

        if (err)
            break;
    }

    SetNomesEstacoes *set_estacoes = criar_set_estacoes();

    InfoParesEstacoes info_pares_estacoes;
    inicializar_pares(&info_pares_estacoes);

    fseek(f, TAM_HEADER, SEEK_SET);

    char buffer[TAM_REGISTRO];

    // Contagem de número de estações únicas e pares válidos
    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        escrever_buffer_na_estacao(buffer, estacao);

        // Se estiver logicamente removido, ignora e vai pro próximo
        if (estacao->removido == '1')
        {
            limpar_estacao(estacao);
            imprimir_estacao(estacao);
            continue;
        }

        // Só inclui e incrementa se a estação ainda não existir no Set
        if (!existe_estacao(set_estacoes, estacao->nomeEstacao))
        {
            incluir_estacao(set_estacoes, estacao->nomeEstacao);
        }

        // Só insere o par se existir uma próxima estação válida
        if (estacao->codProxEstacao != -1)
        {
            inserir_par(&info_pares_estacoes, estacao->codEstacao, estacao->codProxEstacao);
        }

        printf("Qtd Estacoes: %d | Qtd Pares: %d\n", set_estacoes->tamanho, info_pares_estacoes.nroPares);

        limpar_estacao(estacao);
    }

    header->status = '1';
    header->nroEstacoes = set_estacoes->tamanho;
    header->nroParesEstacao = info_pares_estacoes.nroPares;

    escrever_header_no_arquivo(f, header);

    destruir_set_estacoes(set_estacoes);
    destruir_pares(&info_pares_estacoes);

    free(header);

    destruir_estacao(estacao);

    return err;
}
