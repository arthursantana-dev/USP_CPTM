#include "UPDATE.h"

// estacao_valores: valores == 0 -> ignorar e manter;
// valores == -1 -> atualizar para NULO
int UPDATE(char *nome_arquivo_binario, Estacao *estacao_busca, Estacao *estacao_valores, FILE *f)
{

    char buffer[TAM_REGISTRO];

    Header *header = ler_header_do_arquivo(f);

    if (header == NULL)
    {
        free(header);
        mostrar_erro();
        // printf("%d\n", 2);
        return EXIT_FAILURE;
    }

    header->status = '0';
    escrever_header_no_arquivo(f, header);

    // SetNomesEstacoes *set_estacoes = criar_set_estacoes();

    // InfoParesEstacoes info_pares_estacoes;
    // inicializar_pares(&info_pares_estacoes);

    // int encontrou_estacao = 0;

    fseek(f, TAM_HEADER, SEEK_SET);

    int i = -1;

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        i++;
        Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));
        escrever_buffer_na_estacao(buffer, ea);

        if (ea->removido == '1')
        {
            destruir_estacao(ea);
            continue;
        }

        // printf("%s\n", ea->nomeEstacao);

        // if (strcmp("Luz", ea->nomeEstacao) == 0)
        // {
        //     utils_imprimir_estacao(ea);
        //     printf("-------------\n\n");
        // }

        if (comparar_estacoes(estacao_busca, ea))
        {
            // encontrou_estacao = 1;

            editar_estacao(ea, estacao_valores);

            // printf("ACHOU:\n");
            // utils_imprimir_estacao(ea);
            // printf("-------------\n\n");

            escrever_estacao_no_buffer(ea, buffer);

            fseek(f, -TAM_REGISTRO, SEEK_CUR);

            escrever_buffer_no_arquivo(f, buffer);

            fseek(f, 0, SEEK_CUR);
        }

        // incluir_estacao(set_estacoes, ea->nomeEstacao);

        // if (ea->codProxEstacao != -1)
        // {
        //     inserir_par(&info_pares_estacoes, ea->codEstacao, ea->codProxEstacao);
        // }

        destruir_estacao(ea);
    }

    // if (!encontrou_estacao)
    // {
    //     free(header);
    //     destruir_set_estacoes(set_estacoes);
    //     destruir_pares(&info_pares_estacoes);
    //     fclose(f);
    //     // printf("%d\n", 3);

    //     mostrar_erro();
    //     return EXIT_FAILURE;
    // }

    // printf("%d %d\n\n", set_estacoes->tamanho, info_pares_estacoes.nroPares);

    header->status = '1';
    // header->nroEstacoes = set_estacoes->tamanho;
    // header->nroParesEstacao = info_pares_estacoes.nroPares;

    fseek(f, 0, SEEK_SET);

    escrever_header_no_arquivo(f, header);

    free(header);

    return EXIT_SUCCESS;
}
