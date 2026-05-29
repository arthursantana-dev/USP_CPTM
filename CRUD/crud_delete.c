#include "CRUD.h"

int crud_delete(Estacao *estacao_busca, FILE *f)
{

    if (f == NULL)
    {
        mostrar_erro();
        return EXIT_FAILURE;
    }

    Header *header = ler_header_do_arquivo(f);

    if (header == NULL)
    {
        fclose(f);
        mostrar_erro();
        return EXIT_FAILURE;
    }

    char buffer[TAM_REGISTRO];

    int removeu_estacao = 0;

    SetNomesEstacoes *set_estacoes = criar_set_estacoes();

    InfoParesEstacoes info_pares_estacoes;
    inicializar_pares(&info_pares_estacoes);

    header->status = '0';
    escrever_header_no_arquivo(f, header);

    int RRNnovo = 0;

    fseek(f, TAM_HEADER, SEEK_SET);

    int RRN_atual = -1;

    Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {

        escrever_buffer_na_estacao(buffer, ea);

        RRN_atual++;

        if (ea->removido == '1')
        {
            limpar_estacao(ea);
            continue;
        }

        if (!comparar_estacoes(estacao_busca, ea) && ea->codProxEstacao != -1)
        {
            inserir_par(&info_pares_estacoes, ea->codEstacao, ea->codProxEstacao);
        }

        if (!comparar_estacoes(estacao_busca, ea))
        {
            limpar_estacao(ea);
            continue;
        }

        removeu_estacao = 1;

        RRNnovo = RRN_atual;

        ea->removido = '1';
        ea->proximo = header->topo;

        header->topo = RRNnovo;

        escrever_estacao_no_buffer(ea, buffer);

        fseek(f, TAM_HEADER + TAM_REGISTRO * RRNnovo, SEEK_SET);

        escrever_buffer_no_arquivo(f, buffer);

        limpar_estacao(ea);

        fseek(f, 0, SEEK_CUR);
    }

    limpar_estacao(ea);

    if (!removeu_estacao)
    {
        header->status = '1';
        escrever_header_no_arquivo(f, header);
        free(header);
        destruir_set_estacoes(set_estacoes);
        destruir_pares(&info_pares_estacoes);
        destruir_estacao(ea);
        return EXIT_SUCCESS;
    }

    fseek(f, TAM_HEADER, SEEK_SET);

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
    
        escrever_buffer_na_estacao(buffer, ea);

        if (ea->removido == '1')
        {
            limpar_estacao(ea);
            continue;
        }

        incluir_estacao(set_estacoes, ea->nomeEstacao);
        limpar_estacao(ea);
    }

    destruir_estacao(ea);

    header->status = '1';
    header->nroEstacoes = set_estacoes->tamanho;
    header->nroParesEstacao = info_pares_estacoes.nroPares;

    escrever_header_no_arquivo(f, header);

    destruir_set_estacoes(set_estacoes);
    destruir_pares(&info_pares_estacoes);

    free(header);

    return EXIT_SUCCESS;
}

int DELETE(int n, FILE *f)
{

    int err = 0;

    Estacao *estacao = criar_estacao_para_busca(0, "", 0, "", 0, 0, 0, 0);

    for (int i = 0; i < n; i++)
    {

        set_valores_estacao_para_busca(estacao);

        ler_input_para_estacao_de_busca(estacao);
        err = crud_delete(estacao, f);

        limpar_estacao(estacao);

        if (err)
            break;
    }

    destruir_estacao(estacao);

    return err;
}


