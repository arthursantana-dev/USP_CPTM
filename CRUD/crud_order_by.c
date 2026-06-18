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

    if(f_dados == NULL){
        return 1;
    }

    escrever_header_no_arquivo(f_dados, header_dados);

    for (int j = 0; j < i; j++)
    {
        imprimir_estacao(estacoes[j]);
    }

    for(int j = 0; j < i; j++){
        escrever_estacao_no_buffer(estacoes[j], buffer);
        escrever_buffer_no_arquivo(f_dados, buffer);
    }

    for (int j = 0; j < MAX_ESTACOES; j++) {
        destruir_estacao(estacoes[j]);
    }

    destruir_estacao(estacao);

    free(buffer);
    free(header_dados);

    return 0;
}