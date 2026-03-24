#include "estacao.h"

/*cria estação normal: campos iniciados com 0 serão convertidos em -1 (nulos)*/
Estacao *criar_estacao(int codEstacao, char *nomeEstacao, int codLinha, char *nomeLinha, int codProxEstacao, int distProxEstacao, int codLinhaIntegra, int codEstIntegra)
{
    Estacao *estacao = (Estacao *)malloc(sizeof(Estacao));

    if (estacao == NULL)
    {
        return NULL;
    }

    estacao->removido = '0';
    estacao->proximo = -1;
    estacao->codEstacao = codEstacao == 0 ? -1 : codEstacao;
    estacao->codLinha = codLinha == 0 ? -1 : codLinha;
    estacao->codProxEstacao = codProxEstacao == 0 ? -1 : codProxEstacao;
    estacao->distProxEstacao = distProxEstacao == 0 ? -1 : distProxEstacao;
    estacao->codLinhaIntegra = codLinhaIntegra == 0 ? -1 : codLinhaIntegra;
    estacao->codEstIntegra = codEstIntegra == 0 ? -1 : codEstIntegra;
    estacao->tamNomeEstacao = strlen(nomeEstacao);
    estacao->nomeEstacao = nomeEstacao;
    estacao->tamNomeLinha = strlen(nomeLinha);
    estacao->nomeLinha = nomeLinha;

    return estacao;
}

/*cria estação com campos de busca definidos como nulos igual a -1, e 0 para campos que não devem ser considerados para a busca*/
Estacao *criar_estacao_para_busca(int codEstacao, char *nomeEstacao, int codLinha, char *nomeLinha, int codProxEstacao, int distProxEstacao, int codLinhaIntegra, int codEstIntegra)
{
    Estacao *estacao = (Estacao *)malloc(sizeof(Estacao));

    if (estacao == NULL)
    {
        return NULL;
    }

    estacao->removido = '0';
    estacao->proximo = -1;
    estacao->codEstacao = codEstacao;
    estacao->codLinha = codLinha;
    estacao->codProxEstacao = codProxEstacao;
    estacao->distProxEstacao = distProxEstacao;
    estacao->codLinhaIntegra = codLinhaIntegra;
    estacao->codEstIntegra = codEstIntegra;
    estacao->tamNomeEstacao = strlen(nomeEstacao);
    estacao->nomeEstacao = nomeEstacao;
    estacao->tamNomeLinha = strlen(nomeLinha);
    estacao->nomeLinha = nomeLinha;

    return estacao;
}

// comparar estações — ea é estação de busca (campos 0 são desconsiderados; -1 são escolhidos para serem nulos)
int comparar_estacoes(Estacao *ea, Estacao *eb)
{
    printf("codEstacao\n");
    if (ea->codEstacao != 0 && ea->codEstacao != eb->codEstacao)
        return 0;
    printf("codLinha = %d\n", ea->codLinha);

    if (ea->codLinha != 0 && ea->codLinha != eb->codLinha)
        return 0;
    printf("codProxEstacao\n");

    if (ea->codProxEstacao != 0 && ea->codProxEstacao != eb->codProxEstacao)
        return 0;
    printf("distProxEstacao\n");

    if (ea->distProxEstacao != 0 && ea->distProxEstacao != eb->distProxEstacao)
        return 0;
    printf("codLinhaIntegra\n");

    if (ea->codLinhaIntegra != 0 && ea->codLinhaIntegra != eb->codLinhaIntegra)
        return 0;
    printf("codEstIntegra\n");

    if (ea->codEstIntegra != 0 && ea->codEstIntegra != eb->codEstIntegra)
        return 0;
    printf("tamNomeEstacao\n");

    if (ea->tamNomeEstacao != 0 && ea->tamNomeEstacao != eb->tamNomeEstacao)
        return 0;
    printf("tamNomeLinha\n");

    if (ea->tamNomeLinha != 0 && ea->tamNomeLinha != eb->tamNomeLinha)
        return 0;
    printf("nomeEstacao\n");

    if (ea->tamNomeEstacao != 0 && strcmp(ea->nomeEstacao, eb->nomeEstacao) != 0)
        return 0;
    printf("nomeLinha\n");

    if (ea->tamNomeLinha != 0 && strcmp(ea->nomeLinha, eb->nomeLinha) != 0)
        return 0;

    return 1;
}