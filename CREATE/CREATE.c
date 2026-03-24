#include "CREATE.h"

#include "../ParEstacoes/ParEstacoes.h"

int criar_arquivo_binario(const char *nome_arquivo_csv, const char *nome_arquivo_binario)
{
    char *buffer = criar_buffer();

    FILE *csv = fopen(nome_arquivo_csv, "r");
    if (csv == NULL)
    {
        mostrar_erro();
        return EXIT_FAILURE;
    }

    fseek(csv, 101, SEEK_SET); // Pula os primeiros 101 bytes do arquivo CSV

    FILE *bin = fopen(nome_arquivo_binario, "wb+");
    if (bin == NULL)
    {
        mostrar_erro();
        fclose(csv);
        return EXIT_FAILURE;
    }

    Header header = {'1', -1, 0, 0, 0};

    escrever_header_no_arquivo(bin, &header);

    char linha[256];
    // fgets(linha, sizeof(linha), csv);

    int contador_estacoes = 0;
    int total_pares_lidos = 0;

    ParEstacoes pares[MAX_ESTACOES];

    while (fgets(linha, sizeof(linha), csv))
    {
        Estacao *estacao = (Estacao *)malloc(sizeof(Estacao));
        if (estacao == NULL)
        {
            mostrar_erro();
            fclose(csv);
            return EXIT_FAILURE;
        }

        int err = linha_csv_para_estacao(linha, estacao);
        if (err != 0)
        {
            mostrar_erro();
            fclose(csv);
            return EXIT_FAILURE;
        }

        escrever_estacao_no_buffer(estacao, buffer);

        // if (contador_estacoes == 0)
        // {
        //     Estacao *ea = criar_estacao_para_busca(1, "Tucuruvi", 0, "", 2, 992, 0, 0);

        //     utils_imprimir_estacao(estacao);

        //     printf("%d\n\n", comparar_estacoes(ea, estacao));

        // }

        escrever_buffer_no_arquivo(bin, buffer);

        if (estacao->codProxEstacao != -1)
        {
            pares[total_pares_lidos].origem = estacao->codEstacao;
            pares[total_pares_lidos].destino = estacao->codProxEstacao;
            total_pares_lidos++;
        }

        // if(contador_estacoes == 0) {
        // 	utils_mostrar_buffer_como_bytes();
        // }

        contador_estacoes++;

        // utils_imprimir_estacao(estacao);
    }

    /*
        pares[] registra todos os diferentes pares de estacao-proxEstacao.
        para encontrar o número de diferentes pares, ordena-se o vetor e o percorre.
        caso duas linhas consecutivas diferem de origem ou destino, contabiliza +1 pro número de pares.
    */

    qsort(pares, total_pares_lidos, sizeof(ParEstacoes), comparar_pares);

    int nroParesEstacao = 0;
    if (total_pares_lidos > 0)
    {
        nroParesEstacao = 1;
        for (int i = 1; i < total_pares_lidos; i++)
        {
            if (pares[i].origem != pares[i - 1].origem || pares[i].destino != pares[i - 1].destino)
            {
                nroParesEstacao++;
            }
        }
    }

    fseek(bin, 0, SEEK_SET);
    header.nroEstacoes = contador_estacoes;
    header.nroParesEstacao = nroParesEstacao;

    printf("Número de estações: %d\n", header.nroEstacoes);
    printf("Número de pares de estação: %d\n", header.nroParesEstacao);

    escrever_header_no_arquivo(bin, &header);

    utils_mostrar_bytes_do_arquivo(bin, 17);

    fclose(csv);
    fclose(bin);
    return EXIT_SUCCESS;
}