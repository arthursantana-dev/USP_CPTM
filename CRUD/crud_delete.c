#include "CRUD.h"

int busca_sequencial_e_delete(FILE *arquivo_dados, FILE *f_ab, Header *header_dados, Estacao *estacao_busca)
{
    char buffer[TAM_REGISTRO];

    // bool removeu_estacao = false;

    header_btree header_b;

    if (f_ab != NULL) {
        header_b = btree_ler_cabecalho(f_ab);
    }

    int RRNnovo = 0;
    int RRN_atual = -1;

    Estacao *estacao = (Estacao *)calloc(1, sizeof(Estacao));

    // int i = 0;

    fseek(arquivo_dados, TAM_HEADER, SEEK_SET);

    while (fread(buffer, TAM_REGISTRO, 1, arquivo_dados) == 1)
    {

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
        estacao->proximo = header_dados->topo;

        header_dados->topo = RRNnovo;

        escrever_estacao_no_buffer(estacao, buffer);

        fseek(arquivo_dados, TAM_HEADER + TAM_REGISTRO * RRNnovo, SEEK_SET);

        escrever_buffer_no_arquivo(arquivo_dados, buffer);

        // printf("remoção sequencial feita, agora na arvore\n");
        if (f_ab != NULL) {
            btree_remover(f_ab, &header_b, estacao->codEstacao);
        }

        limpar_estacao(estacao);
    }

    destruir_estacao(estacao);

    return EXIT_SUCCESS;
}

int busca_em_indice_e_delete(FILE *arquivo_dados, FILE *f_ab, Header *header_dados, Estacao *estacao_busca)
{
    header_btree header_b = btree_ler_cabecalho(f_ab);

    // recupera o byte offset físico consultando a árvore b
    int byte_offset = btree_buscar(f_ab, &header_b, estacao_busca->codEstacao);

    // aborta a deleção se a chave primária não existir no índice
    if (byte_offset == -1)
    {
        return EXIT_SUCCESS;
    }

    char buffer[TAM_REGISTRO];
    Estacao *estacao = (Estacao *)calloc(1, sizeof(Estacao));

    // salta cirurgicamente para a posição do registro no arquivo de dados
    fseek(arquivo_dados, byte_offset, SEEK_SET);
    if (fread(buffer, TAM_REGISTRO, 1, arquivo_dados) == 1)
    {
        escrever_buffer_na_estacao(buffer, estacao);

        // garante que o registro não foi apagado previamente e que os outros campos coincidem
        if (estacao->removido == '0' && comparar_estacoes(estacao_busca, estacao))
        {
            // deduz o rrn a partir do byte offset para atualizar a pilha
            int rrn_atual = (byte_offset - TAM_HEADER) / TAM_REGISTRO;

            // efetua a remoção lógica e encadeia o registro atual no topo da lista
            estacao->removido = '1';
            estacao->proximo = header_dados->topo;
            header_dados->topo = rrn_atual;

            // volta o cursor para sobrescrever apenas o registro alvo
            escrever_estacao_no_buffer(estacao, buffer);
            fseek(arquivo_dados, byte_offset, SEEK_SET);
            escrever_buffer_no_arquivo(arquivo_dados, buffer);

            // espelha a remoção na árvore b para manter a sincronia
            btree_remover(f_ab, &header_b, estacao_busca->codEstacao);
        }

        limpar_estacao(estacao);
    }

    destruir_estacao(estacao);

    return EXIT_SUCCESS;
}
int crud_delete(Estacao *estacao_busca, FILE *f_dados, FILE *f_ab, Header *header_dados)
{

    // int removeu_estacao = 0;
    int erro;

    // se o arquivo de índice for fornecido e a estação de busca tiver um código válido, tenta remover usando o índice, senão faz a busca sequencial
    if (f_ab != NULL && estacao_busca->codEstacao != -2 && estacao_busca->codEstacao != 0)
    {
        erro = busca_em_indice_e_delete(f_dados, f_ab, header_dados, estacao_busca);
    }
    else
    {
        erro = busca_sequencial_e_delete(f_dados, f_ab, header_dados, estacao_busca);
    }
    if (erro)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int DELETE(int n, FILE *f_dados, FILE *f_ab)
{

    if (f_dados== NULL)
    {
        return EXIT_FAILURE;
    }

    int err = 0;

    Estacao *estacao = criar_estacao_para_busca(-2, "", -2, "", -2, -2, -2, -2);

    Header *header_dados = ler_header_do_arquivo(f_dados);

    if (header_dados == NULL)
    {
        return EXIT_FAILURE;
    }

    header_dados->status = '0';
    escrever_header_no_arquivo(f_dados, header_dados);

    for (int i = 0; i < n; i++)
    {

        set_valores_estacao_para_busca(estacao);

        ler_input_para_estacao_de_busca(estacao);

        // printf("Estação a ser buscada: ");
        // imprimir_estacao(estacao);

        // repassa o ponteiro da árvore para que a remoção secundária ocorra internamente se possível
        err = crud_delete(estacao, f_dados, f_ab, header_dados);

        limpar_estacao(estacao);

        if (err)
            break;
    }

    header_dados->status = '1';
    escrever_header_no_arquivo(f_dados, header_dados);
    destruir_estacao(estacao);

    free(header_dados);

    // printf("Número de estações quaisquer: %d\n", numero_estacoes);

    return err;
}
