#include "../ParEstacoes/ParEstacoes.h"
#include "../SetNomes/SetNomes.h"

#include "_select.c"
#include "_delete.c"
#include "_insert.c"
#include "_update.c"

/* =========================================================
 * CREATE
 * ========================================================= */

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
        }

        contador_estacoes++;

        free(estacao->nomeEstacao);
        free(estacao->nomeLinha);
        free(estacao);
    }

    fseek(bin, 0, SEEK_SET);
    header.status = '1';
    header.nroParesEstacao = info_pares_estacoes.nroPares;
    header.proxRRN = contador_estacoes;
    header.nroEstacoes = set_estacoes->tamanho;

    escrever_header_no_arquivo(bin, &header);

    destruir_pares(&info_pares_estacoes);

    destruir_set_estacoes(set_estacoes);

    free(buffer);

    fclose(bin);

    BinarioNaTela(nome_arquivo_binario);

    return EXIT_SUCCESS;
}

/* =========================================================
 * DELETE
 * ========================================================= */


int DELETE(int n, FILE *f)
{

    int err = 0;

    Estacao *estacao = criar_estacao_para_busca(0, "", 0, "", 0, 0, 0, 0);

    for (int i = 0; i < n; i++)
    {

        set_valores_estacao_para_busca(estacao);

        ler_input_para_estacao_de_busca(estacao);
        err = _delete(estacao, f);

        limpar_estacao(estacao);

        if (err)
            break;
    }

    destruir_estacao(estacao);

    return err;
}


/* =========================================================
 * INSERT
 * ========================================================= */


int INSERT(FILE *f)
{
    if (f == NULL)
        return 1;

    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        int codEstacao, codLinha, codProxEstacao, distProxEstacao, codLinhaIntegra, codEstacaoIntegra;
        char *nomeEstacao = (char *)malloc(MAX_TAM_NOME * sizeof(char));
        char *nomeLinha = (char *)malloc(MAX_TAM_NOME * sizeof(char));
        scanf("%d", &codEstacao);
        ScanQuoteString(nomeEstacao);
        nullOrInt(&codLinha);
        nullOrString(nomeLinha); // nome nulo é "-1" pra ficar mais fácil pro meu select
        nullOrInt(&codProxEstacao);
        nullOrInt(&distProxEstacao);
        nullOrInt(&codLinhaIntegra);
        nullOrInt(&codEstacaoIntegra);
        Estacao *estacao = criar_estacao(codEstacao, nomeEstacao, codLinha, nomeLinha, codProxEstacao, distProxEstacao, codLinhaIntegra, codEstacaoIntegra);
        if (_insert(f, estacao))
        {
            destruir_estacao(estacao);
            return EXIT_FAILURE;
        }
        destruir_estacao(estacao);
    }

    return 0;
}

/* =========================================================
 * SELECT
 * ========================================================= */

int SELECT_ALL(FILE *f)
{
    // Abrindo arquivo binário, lendo header, verificações etc
    char buffer[TAM_REGISTRO];
    if (f == NULL)
    {
        return EXIT_FAILURE;
    }
    Header *header = ler_header_do_arquivo(f);
    if (header == NULL)
    {
        return EXIT_FAILURE;
    }
    int nroEstacoes = header->nroEstacoes;
    if (nroEstacoes == 0)
    {
        printf("Registro inexistente.\n");
        free(header);
        return 0; // erro tratado localmente, sem necessidade de flag
    }

    fseek(f, TAM_HEADER, SEEK_SET);

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        Estacao *ea = (Estacao *)malloc(sizeof(Estacao));
        escrever_buffer_na_estacao(buffer, ea);
        if (ea->removido == '1')
        {
            destruir_estacao(ea);
            continue;
        }
        utils_imprimir_estacao_ln(ea);

        destruir_estacao(ea);
    }

    free(header);
    return 0;
}

int SELECT(int n, FILE *f)
{
    int err = 0;

    Estacao *estacao_selecao = criar_estacao_para_busca(0, "", 0, "", 0, 0, 0, 0);

    for (int i = 0; i < n; i++)
    {

        set_valores_estacao_para_busca(estacao_selecao);

        ler_input_para_estacao_de_busca(estacao_selecao);
        err = _select(estacao_selecao, f);

        limpar_estacao(estacao_selecao);

        if (err)
            break;

        printf("\n");
    }

    destruir_estacao(estacao_selecao);

    return err;
}

/* =========================================================
 * UPDATE
 * ========================================================= */

int UPDATE(int n, FILE *f)
{
    int err = 0;

    Estacao *estacao_busca = criar_estacao_para_busca(0, "", 0, "", 0, 0, 0, 0);

    Estacao *estacao_valores = criar_estacao_para_busca(0, "", 0, "", 0, 0, 0, 0);

    for (int i = 0; i < n; i++)
    {

        set_valores_estacao_para_busca(estacao_busca);
        set_valores_estacao_para_busca(estacao_valores);

        ler_input_para_estacao_de_busca(estacao_busca);
        ler_input_para_estacao_de_busca(estacao_valores);

        err = _update(estacao_busca, estacao_valores, f);

        limpar_estacao(estacao_busca);
        limpar_estacao(estacao_valores);

        if (err)
            break;

        printf("\n");
    }

    destruir_estacao(estacao_busca);
    destruir_estacao(estacao_valores);

    return err;

}