#include "CRUD.h"

int ORDER_BY(FILE *f_dados, char *campo_ordenacao, char *nome_arquivo_ordenado)
{
    Estacao *estacoes[MAX_ESTACOES];

    if (f_dados == NULL)
    {
        return 1;
    }

    Header *header_dados = ler_header_do_arquivo(f_dados);

    char *buffer = criar_buffer();

    Estacao *estacao = criar_estacao_para_busca(-2, "", -2, "", -2, -2, -2, -2);

    int i = 0;

    // popula estacoes[] com estações vazias
    for (int i = 0; i < MAX_ESTACOES; i++)
    {
        estacoes[i] = criar_estacao_para_busca(-2, "", -2, "", -2, -2, -2, -2);
    }

    fseek(f_dados, TAM_HEADER, SEEK_SET);

    while (fread(buffer, TAM_REGISTRO, 1, f_dados) == 1)
    {

        escrever_buffer_na_estacao(buffer, estacao);
        if (estacao->removido == '1')
        {
            limpar_estacao(estacao);
            continue;
        }

        // copia a estação lida para o vetor na RAM
        copiar_estacao(estacoes[i], estacao);

        i++;

        limpar_estacao(estacao);
    }

    // ordena de acordo com o campo pedido - codEstacao ou codProxEstacao
    ordenar_estacoes(estacoes, i, campo_ordenacao);

    // caso de registros removidos: atualiza o proxRRN
    header_dados->proxRRN = i;

    f_dados = freopen(nome_arquivo_ordenado, "wb", f_dados);

    if (f_dados == NULL)
    {
        return 1;
    }

    escrever_header_no_arquivo(f_dados, header_dados);

    // for (int j = 0; j < i; j++)
    // {
    //     imprimir_estacao(estacoes[j]);
    // }

    for (int j = 0; j < i; j++)
    {
        escrever_estacao_no_buffer(estacoes[j], buffer);
        escrever_buffer_no_arquivo(f_dados, buffer);
    }

    for (int j = 0; j < MAX_ESTACOES; j++)
    {
        destruir_estacao(estacoes[j]);
    }

    destruir_estacao(estacao);

    free(buffer);
    free(header_dados);

    return 0;
}

#include "CRUD.h"

int SORT_MERGE_JOIN(char *nome_arquivo_binario_A, char *nome_arquivo_binario_B)
{
    FILE *f_dados_A = fopen(nome_arquivo_binario_A, "rb+");
    FILE *f_dados_B = fopen(nome_arquivo_binario_B, "rb+");

    if (f_dados_A == NULL || f_dados_B == NULL)
    {
        if (f_dados_A) fclose(f_dados_A);
        if (f_dados_B) fclose(f_dados_B);
        return EXIT_FAILURE;
    }

    // ordena o arquivo a pela chave primaria
    int err = ORDER_BY(f_dados_A, "codEstacao", nome_arquivo_binario_A);
    if (err)
    {
        fclose(f_dados_B);
        return EXIT_FAILURE;
    }
    
    // o freopen do order_by deixou o arquivo a em modo wb, precisamos fecha-lo
    fclose(f_dados_A);

    // ordena o arquivo b pela chave estrangeira
    err = ORDER_BY(f_dados_B, "codProxEstacao", nome_arquivo_binario_B);
    if (err)
    {
        return EXIT_FAILURE;
    }
    
    // fecha o arquivo b que tambem ficou em modo wb
    fclose(f_dados_B);

    // reabre os dois arquivos recem-ordenados no modo de leitura estrita
    f_dados_A = fopen(nome_arquivo_binario_A, "rb");
    f_dados_B = fopen(nome_arquivo_binario_B, "rb");

    if (f_dados_A == NULL || f_dados_B == NULL)
    {
        if (f_dados_A) fclose(f_dados_A);
        if (f_dados_B) fclose(f_dados_B);
        return EXIT_FAILURE;
    }

    // pula o cabecalho de ambos os arquivos para cair direto nos dados
    fseek(f_dados_A, TAM_HEADER, SEEK_SET);
    fseek(f_dados_B, TAM_HEADER, SEEK_SET);

    char buffer_A[TAM_REGISTRO];
    char buffer_B[TAM_REGISTRO];

    Estacao *estacao_A = criar_estacao_para_busca(-2, "", -2, "", -2, -2, -2, -2);
    Estacao *estacao_B = criar_estacao_para_busca(-2, "", -2, "", -2, -2, -2, -2);

    // carrega o primeiro registro de ambos os arquivos
    bool leu_A = (fread(buffer_A, TAM_REGISTRO, 1, f_dados_A) == 1);
    if (leu_A) escrever_buffer_na_estacao(buffer_A, estacao_A);

    bool leu_B = (fread(buffer_B, TAM_REGISTRO, 1, f_dados_B) == 1);
    if (leu_B) escrever_buffer_na_estacao(buffer_B, estacao_B);

    bool encontrou_match = false;

    // loop principal: encerra se qualquer um dos arquivos chegar ao fim (eof)
    while (leu_A && leu_B)
    {
        // verifica o match perfeito entre a pk de a e a fk de b
        if (estacao_A->codEstacao == estacao_B->codProxEstacao)
        {
            imprimir_join_estacoes(estacao_B, estacao_A);
            encontrou_match = true;

            // como a relacao e 1:n (varias estacoes podem apontar para a mesma proxima), avancamos apenas o b
            limpar_estacao(estacao_B);
            leu_B = (fread(buffer_B, TAM_REGISTRO, 1, f_dados_B) == 1);
            if (leu_B) escrever_buffer_na_estacao(buffer_B, estacao_B);
        }
        else if (estacao_A->codEstacao < estacao_B->codProxEstacao)
        {
            // a chave de a esta atrasada, entao avancamos a
            limpar_estacao(estacao_A);
            leu_A = (fread(buffer_A, TAM_REGISTRO, 1, f_dados_A) == 1);
            if (leu_A) escrever_buffer_na_estacao(buffer_A, estacao_A);
        }
        else
        {
            // a chave de b esta atrasada, entao avancamos b
            limpar_estacao(estacao_B);
            leu_B = (fread(buffer_B, TAM_REGISTRO, 1, f_dados_B) == 1);
            if (leu_B) escrever_buffer_na_estacao(buffer_B, estacao_B);
        }
    }

    if (!encontrou_match)
    {
        printf("Registro inexistente.\n");
    }

    // limpeza final de memoria
    destruir_estacao(estacao_A);
    destruir_estacao(estacao_B);

    fclose(f_dados_A);
    fclose(f_dados_B);

    return EXIT_SUCCESS;
}