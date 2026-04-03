#include "Estacao.h"

/*cria estação normal: campos iniciados com 0 serão convertidos em -1 (nulos)*/
Estacao *criar_estacao(int codEstacao, char *nomeEstacao, int codLinha, char *nomeLinha, int codProxEstacao, int distProxEstacao, int codLinhaIntegra, int codEstacaoIntegra)
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
    estacao->codEstacaoIntegra = codEstacaoIntegra == 0 ? -1 : codEstacaoIntegra;
    estacao->tamNomeEstacao = strlen(nomeEstacao);
    estacao->nomeEstacao = nomeEstacao;
    estacao->tamNomeLinha = strlen(nomeLinha);
    estacao->nomeLinha = nomeLinha;

    return estacao;
}

/*cria estação com campos de busca definidos como nulos igual a -1, e 0 para campos que não devem ser considerados para a busca*/
Estacao *criar_estacao_para_busca(int codEstacao, char *nomeEstacao, int codLinha, char *nomeLinha, int codProxEstacao, int distProxEstacao, int codLinhaIntegra, int codEstacaoIntegra)
{
    Estacao *estacao = (Estacao *)calloc(1,sizeof(Estacao));

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
    estacao->codEstacaoIntegra = codEstacaoIntegra;
    // estacao->tamNomeEstacao = strlen(nomeEstacao);
    // estacao->nomeEstacao = nomeEstacao;
    // estacao->tamNomeLinha = strlen(nomeLinha);
    // estacao->nomeLinha = nomeLinha;

    if (nomeEstacao != NULL) {
        estacao->tamNomeEstacao = strlen(nomeEstacao);
        estacao->nomeEstacao = (char *)malloc((estacao->tamNomeEstacao + 1) * sizeof(char));
        strcpy(estacao->nomeEstacao, nomeEstacao);
    } else {
        estacao->tamNomeEstacao = 0;
        estacao->nomeEstacao = NULL;
    }

    if (nomeLinha != NULL) {
        estacao->tamNomeLinha = strlen(nomeLinha);
        estacao->nomeLinha = (char *)malloc((estacao->tamNomeLinha + 1) * sizeof(char));
        strcpy(estacao->nomeLinha, nomeLinha);
    } else {
        estacao->tamNomeLinha = 0;
        estacao->nomeLinha = NULL;
    }

    return estacao;
}

// comparar estações — ea é estação de busca (campos 0 são desconsiderados; -1 são escolhidos para serem nulos)
int comparar_estacoes(Estacao *ea, Estacao *eb)
{
    // printf("codEstacao\n");
    if (ea->codEstacao != 0 && ea->codEstacao != eb->codEstacao)
        return 0;
    // printf("codLinha = %d\n", ea->codLinha);

    if (ea->codLinha != 0 && ea->codLinha != eb->codLinha)
        return 0;
    // printf("codProxEstacao\n");

    if (ea->codProxEstacao != 0 && ea->codProxEstacao != eb->codProxEstacao)
        return 0;
    // printf("distProxEstacao\n");

    if (ea->distProxEstacao != 0 && ea->distProxEstacao != eb->distProxEstacao)
        return 0;
    // printf("codLinhaIntegra\n");

    if (ea->codLinhaIntegra != 0 && ea->codLinhaIntegra != eb->codLinhaIntegra)
        return 0;
    // printf("codEstacaoIntegra\n");

    if (ea->codEstacaoIntegra != 0 && ea->codEstacaoIntegra != eb->codEstacaoIntegra)
        return 0;
    // printf("tamNomeEstacao\n");
    // printf("%d (%s) %d (%s)\n", ea->tamNomeEstacao, ea->nomeEstacao, eb->tamNomeEstacao, eb->nomeEstacao);

    if (ea->tamNomeEstacao != 0 && ea->tamNomeEstacao != eb->tamNomeEstacao)
        return 0;
    // printf("tamNomeLinha\n");

    if (ea->tamNomeLinha != 0 && ea->tamNomeLinha != eb->tamNomeLinha)
        return 0;
    // printf("nomeEstacao\n");

    if (ea->tamNomeEstacao != 0 && strcmp(ea->nomeEstacao, eb->nomeEstacao) != 0)
        return 0;
    // printf("nomeLinha\n");

    if (ea->tamNomeLinha != 0 && strcmp(ea->nomeLinha, eb->nomeLinha) != 0)
        return 0;

    // printf("ea: \n");

    // printf("Encontrou\n");

    return 1;
}

void editar_estacao(Estacao *estacao, Estacao *novos_valores)
{
    if (novos_valores->codEstacao != 0)
        estacao->codEstacao = novos_valores->codEstacao;

    if (novos_valores->codLinha != 0)
        estacao->codLinha = novos_valores->codLinha;

    if (novos_valores->codProxEstacao != 0)
        estacao->codProxEstacao = novos_valores->codProxEstacao;

    if (novos_valores->distProxEstacao != 0)
        estacao->distProxEstacao = novos_valores->distProxEstacao;

    if (novos_valores->codLinhaIntegra != 0)
        estacao->codLinhaIntegra = novos_valores->codLinhaIntegra;

    if (novos_valores->codEstacaoIntegra != 0)
        estacao->codEstacaoIntegra = novos_valores->codEstacaoIntegra;

    if (novos_valores->tamNomeEstacao != 0)
    {
        free(estacao->nomeEstacao);
        estacao->tamNomeEstacao = novos_valores->tamNomeEstacao;

        estacao->nomeEstacao = (char *)malloc((estacao->tamNomeEstacao + 1) * sizeof(char));

        strcpy(estacao->nomeEstacao, novos_valores->nomeEstacao);
    }

    if (novos_valores->tamNomeLinha != 0)
    {
        free(estacao->nomeLinha);
        estacao->tamNomeLinha = novos_valores->tamNomeLinha;

        estacao->nomeLinha = (char *)malloc((estacao->tamNomeLinha + 1) * sizeof(char));

        strcpy(estacao->nomeLinha, novos_valores->nomeLinha);
    }

}

void destruir_estacao(Estacao *estacao)
{
    if (estacao != NULL)
    {
        free(estacao->nomeEstacao);
        free(estacao->nomeLinha);
        free(estacao);
    }
}