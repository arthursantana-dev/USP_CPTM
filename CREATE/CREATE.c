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

    char cabecalho[256];
    fgets(cabecalho, sizeof(cabecalho), csv);

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

    fseek(bin, 0, SEEK_SET);
    // APAGAR O COMENTARIO: header.status = '1';
    header.nroParesEstacao = info_pares_estacoes.nroPares;
    header.proxRRN = contador_estacoes;
    header.nroEstacoes = set_estacoes->tamanho;

    escrever_header_no_arquivo(bin, &header);

    destruir_pares(&info_pares_estacoes);

    destruir_set_estacoes(set_estacoes);

    free(buffer);

    fclose(csv);
    fclose(bin);

    BinarioNaTela(nome_arquivo_binario);

    return EXIT_SUCCESS;
}