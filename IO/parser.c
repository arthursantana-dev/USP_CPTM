#include "IO.h"

char *obter_proximo_campo_csv(char **ponteiro_linha)
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

int linha_csv_para_estacao(char *linha_csv, Estacao *estacao)
{
    char *ponteiro_linha = linha_csv;
    char *token;

    estacao->removido = '0';
    estacao->proximo = -1;

    token = obter_proximo_campo_csv(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->codEstacao = atoi(token);

    token = obter_proximo_campo_csv(&ponteiro_linha);
    if (token == NULL)
        return -1;

    // normalizar_utf8(token);

    estacao->tamNomeEstacao = strlen(token);
    estacao->nomeEstacao = strdup(token);

    token = obter_proximo_campo_csv(&ponteiro_linha);
    if (token == NULL)
        return -1;

    // normalizar_utf8(token);

    estacao->codLinha = atoi(token) == 0 ? -1 : atoi(token);

    token = obter_proximo_campo_csv(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->tamNomeLinha = strlen(token);
    estacao->nomeLinha = strdup(token);

    token = obter_proximo_campo_csv(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->codProxEstacao = atoi(token) == 0 ? -1 : atoi(token);

    token = obter_proximo_campo_csv(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->distProxEstacao = atoi(token) == 0 ? -1 : atoi(token);

    token = obter_proximo_campo_csv(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->codLinhaIntegra = atoi(token) == 0 ? -1 : atoi(token);

    token = obter_proximo_campo_csv(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->codEstacaoIntegra = atoi(token) == 0 ? -1 : atoi(token);

    return 0;
}

void input_para_estacao(Estacao *estacao, char *linha)
{
    char *elementos[MAX_TOKENS];

    int qtd_elementos = decompor_linha(linha, elementos);

    vetor_char_para_estacao(estacao, elementos, qtd_elementos);
}

void ler_input_para_estacao_de_busca(Estacao *estacao)
{
    int num_campos;

    if (scanf("%d", &num_campos) != 1)
        return;

    char chaves[10][50];
    char valores[10][256];

    char *elementos[20];
    int qtd_elementos = 0;

    for (int i = 0; i < num_campos; i++)
    {
        scanf("%s", chaves[i]);
        elementos[qtd_elementos++] = chaves[i];

        ScanQuoteString(valores[i]);
        if (strcmp("", valores[i]) == 0)
        {
            memcpy(valores[i], "-1", sizeof("-1"));
        }
        elementos[qtd_elementos++] = valores[i];
    }

    vetor_char_para_estacao(estacao, elementos, qtd_elementos);
}