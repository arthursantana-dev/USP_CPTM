#include "DELETE.h"

int DELETE(Estacao *estacao_busca, FILE *f)
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

    int rrn_atual = -1;

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));

        escrever_buffer_na_estacao(buffer, ea);

        rrn_atual++;

        if (ea->removido == '1')
        {
            destruir_estacao(ea);
            continue;
        }

        if (!comparar_estacoes(estacao_busca, ea) && ea->codProxEstacao != -1)
        {
            inserir_par(&info_pares_estacoes, ea->codEstacao, ea->codProxEstacao);
        }

        if (!comparar_estacoes(estacao_busca, ea))
        {
            destruir_estacao(ea);
            continue;
        }

        removeu_estacao = 1;

        RRNnovo = rrn_atual;

        ea->removido = '1';
        ea->proximo = header->topo;

        header->topo = RRNnovo;

        escrever_estacao_no_buffer(ea, buffer);

        fseek(f, TAM_HEADER + TAM_REGISTRO * RRNnovo, SEEK_SET);

        escrever_buffer_no_arquivo(f, buffer);

        destruir_estacao(ea);

        fseek(f, 0, SEEK_CUR);
    }

    if (!removeu_estacao)
    {
        header->status = '1';
        escrever_header_no_arquivo(f, header);
        free(header);
        destruir_set_estacoes(set_estacoes);
        destruir_pares(&info_pares_estacoes);
        return EXIT_SUCCESS;
    }

    fseek(f, TAM_HEADER, SEEK_SET);

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));

        escrever_buffer_na_estacao(buffer, ea);

        if (ea->removido == '1')
        {
            destruir_estacao(ea);
            continue;
        }

        incluir_estacao(set_estacoes, ea->nomeEstacao);
        destruir_estacao(ea);
    }

    header->status = '1';
    header->nroEstacoes = set_estacoes->tamanho;
    header->nroParesEstacao = info_pares_estacoes.nroPares;

    escrever_header_no_arquivo(f, header);

    destruir_set_estacoes(set_estacoes);
    destruir_pares(&info_pares_estacoes);

    free(header);

    return EXIT_SUCCESS;
}