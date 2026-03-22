#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../CONSTS.h"

#include "../Header/Header.h"
#include "../Estacao/Estacao.h"
#include "../utils/utils.h"

// leitura dos campos do arquivo .csv
char *obter_proximo_campo(char **ponteiro_linha)
{
    char *inicio = *ponteiro_linha;

    if (inicio == NULL)
        return NULL;

    char *virgula = strchr(inicio, ',');

    if (virgula != NULL)
    {
        *virgula = '\0';
        *ponteiro_linha = virgula + 1;
    }
    else
    {
        char *quebra_linha = strpbrk(inicio, "\r\n");
        if (quebra_linha)
            *quebra_linha = '\0';

        *ponteiro_linha = NULL;
    }

    return inicio;
}

// CodEstacao,NomeEstacao,CodLinha,NomeLinha,CodProxEst,DistanciaProxEst,CodLinhaInteg,CodEstacaoInteg
int linha_csv_para_estacao(char *linha_csv, Estacao *estacao)
{
    char *ponteiro_linha = linha_csv;
    char *token;

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->codEstacao = atoi(token);

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->tamNomeEstacao = strlen(token);
    estacao->nomeEstacao = strdup(token);

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->codLinha = atoi(token) == 0 ? -1 : atoi(token);

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->tamNomeLinha = strlen(token);
    estacao->nomeLinha = strdup(token);

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->codProxEstacao = atoi(token) == 0 ? -1 : atoi(token);

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->distProxEstacao = atoi(token) == 0 ? -1 : atoi(token);

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->codLinhaIntegra = atoi(token) == 0 ? -1 : atoi(token);

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->codEstIntegra = atoi(token) == 0 ? -1 : atoi(token);

    return 0;
}

void escrever_buffer_no_arquivo(FILE *f, char *buffer)
{
    fwrite(buffer, sizeof(char), TAM_REGISTRO, f);
}

int escrever_header_no_arquivo(FILE *f, Header *header)
{
    fseek(f, 0, SEEK_SET);
    fwrite(&header->status, sizeof(header->status), 1, f);
    fwrite(&header->topo, sizeof(header->topo), 1, f);
    fwrite(&header->proxRRN, sizeof(header->proxRRN), 1, f);
    fwrite(&header->nroEstacoes, sizeof(header->nroEstacoes), 1, f);
    fwrite(&header->nroParesEstacao, sizeof(header->nroParesEstacao), 1, f);
    return 0;
}

Header *ler_header_do_arquivo(FILE *f)
{
    Header *header = (Header *)malloc(sizeof(Header));
    fseek(f, 0, SEEK_SET);
    if (fread(&header->status, sizeof(header->status), 1, f) != 1)
    {
        mostrar_erro();
        return NULL;
    }
    if (fread(&header->topo, sizeof(header->topo), 1, f) != 1)
    {
        mostrar_erro();
        return NULL;
    }
    if (fread(&header->proxRRN, sizeof(header->proxRRN), 1, f) != 1)
    {
        mostrar_erro();
        return NULL;
    }
    if (fread(&header->nroEstacoes, sizeof(header->nroEstacoes), 1, f) != 1)
    {
        mostrar_erro();
        return NULL;
    }
    if (fread(&header->nroParesEstacao, sizeof(header->nroParesEstacao), 1, f) != 1)
    {
        mostrar_erro();
        return NULL;
    }
    return header;
}