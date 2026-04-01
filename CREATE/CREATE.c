#include "CREATE.h"

#include "../ParEstacoes/ParEstacoes.h"
#include "../SetNomes/SetNomes.h"

int CREATE(char *nome_arquivo_csv, char *nome_arquivo_binario)
{

    char *buffer = criar_buffer();

    FILE *csv = fopen(nome_arquivo_csv, "r");
    if (csv == NULL)
    {
        mostrar_erro();
        return EXIT_FAILURE;
    }

    fseek(csv, 101, SEEK_SET);

    FILE *bin = fopen(nome_arquivo_binario, "wb+");
    if (bin == NULL)
    {
        free(buffer);
        mostrar_erro();
        fclose(csv);
        return EXIT_FAILURE;
    }

    SetNomesEstacoes *set_estacoes = criar_set_estacoes();

    // estação inconsistente: status = '0' no header, e só é setado para '1' ao final da criação do arquivo
    Header header = {'0', -1, 0, 0, 0};

    escrever_header_no_arquivo(bin, &header);

    InfoParesEstacoes info_pares_estacoes;
    inicializar_pares(&info_pares_estacoes);

    char linha[256];

    int contador_estacoes = 0;
    int total_pares_lidos = 0;

    while (fgets(linha, sizeof(linha), csv))
    {
        Estacao *estacao = (Estacao *)malloc(sizeof(Estacao));
        if (estacao == NULL)
        {
            free(buffer);
            destruir_pares(&info_pares_estacoes);

            destruir_set_estacoes(set_estacoes);
            free(set_estacoes);

            mostrar_erro();
            fclose(csv);
            return EXIT_FAILURE;
        }

        int err = linha_csv_para_estacao(linha, estacao);
        if (err != 0)
        {
            free(estacao->nomeEstacao);
            free(estacao->nomeLinha);
            free(estacao);

            free(buffer);
            destruir_pares(&info_pares_estacoes);

            destruir_set_estacoes(set_estacoes);
            free(set_estacoes);
            mostrar_erro();
            fclose(csv);
            return EXIT_FAILURE;
        }

        escrever_estacao_no_buffer(estacao, buffer);

        if (!existe_estacao(set_estacoes, estacao->nomeEstacao))
        {
            incluir_estacao(set_estacoes, estacao->nomeEstacao);
        }

        escrever_buffer_no_arquivo(bin, buffer);

        if (estacao->codProxEstacao != -1)
        {
            inserir_par(&info_pares_estacoes, estacao->codEstacao, estacao->codProxEstacao);
            total_pares_lidos++;
        }

        contador_estacoes++;

        free(estacao->nomeEstacao);
        free(estacao->nomeLinha);
        free(estacao);
    }

    /*
        pares[] registra todos os diferentes pares de estacao-proxEstacao.
        para encontrar o número de diferentes pares, ordena-se o vetor e o percorre.
        caso duas linhas consecutivas diferem de origem ou destino, contabiliza +1 pro número de pares.
    */

    ordenar_pares(&info_pares_estacoes);

    ParEstacoes *par_i;

    iniciar_iterador(&info_pares_estacoes, &par_i);

    int nroParesEstacao = 0;
    if (total_pares_lidos > 0)
    {
        nroParesEstacao = 1;

        int i = 0;

        while (par_i != NULL)
        {

            if (par_i->prox == NULL)
            {
                break;
            }

            if (par_i->origem != par_i->prox->origem ||
                par_i->destino != par_i->prox->destino)
            {
                nroParesEstacao++;
            }

            par_i = par_i->prox;
            i++;
        }
    }

    fseek(bin, 0, SEEK_SET);
    header.nroParesEstacao = nroParesEstacao;
    header.proxRRN = contador_estacoes;
    header.nroEstacoes = set_estacoes->tamanho;

    // printf("Número de pares de estações: %d\n", nroParesEstacao);
    // printf("Numero de registros de estação: %d\n", set_estacoes->tamanho);

    // printf("Número de estações: %d\n", header.nroEstacoes);
    // printf("Número de pares de estação: %d\n", header.nroParesEstacao);

    // printf("%d %d\n\n", set_estacoes->tamanho, info_pares_estacoes.nroPares);

    escrever_header_no_arquivo(bin, &header);

    // utils_mostrar_bytes_do_arquivo(bin, 17);

    destruir_pares(&info_pares_estacoes);

    destruir_set_estacoes(set_estacoes);

    free(buffer);

    fclose(csv);
    fclose(bin);

    BinarioNaTela(nome_arquivo_binario);

    return EXIT_SUCCESS;
}