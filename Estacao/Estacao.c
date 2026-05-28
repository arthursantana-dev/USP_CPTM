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
    estacao->tamNomeEstacao = strlen(nomeEstacao);
    estacao->nomeEstacao = strdup(nomeEstacao);
    estacao->tamNomeLinha = strlen(nomeLinha);
    estacao->nomeLinha = strdup(nomeLinha);

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

void copiar_estacao(Estacao *estacao, Estacao *novos_valores)
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
        limpar_estacao(estacao);
        free(estacao);
    }
}


//Diferentemente do destruir_estacao, não libera a struct
void limpar_estacao(Estacao *estacao){
    if (estacao != NULL)
    {
        if(estacao->nomeEstacao != NULL) {
            free(estacao->nomeEstacao);
            estacao->nomeEstacao = NULL;
        }
        
        if(estacao->nomeLinha != NULL) {
            free(estacao->nomeLinha);
            estacao->nomeLinha = NULL;
        }
    }
}

void set_valores_estacao_para_busca(Estacao *estacao){
    estacao->codEstacao = 0;
    estacao->codLinha = 0;
    estacao->codProxEstacao = 0;
    estacao->distProxEstacao = 0;
    estacao->codLinhaIntegra = 0;
    estacao->codEstacaoIntegra = 0;
    estacao->tamNomeEstacao = 0;
    if(estacao->nomeEstacao != NULL) {
        free(estacao->nomeEstacao);
        estacao->nomeEstacao = NULL;
    }
    estacao->tamNomeLinha = 0;
    if(estacao->nomeLinha != NULL) {
        free(estacao->nomeLinha);
        estacao->nomeLinha = NULL;
    }
}

void set_estacao(Estacao *estacao, int codEstacao, char *nomeEstacao, int codLinha, char *nomeLinha, int codProxEstacao, int distProxEstacao, int codLinhaIntegra, int codEstacaoIntegra){
    
    estacao->codEstacao = codEstacao == 0 ? -1 : codEstacao;
    estacao->codLinha = codLinha == 0 ? -1 : codLinha;
    estacao->codProxEstacao = codProxEstacao == 0 ? -1 : codProxEstacao;
    estacao->distProxEstacao = distProxEstacao == 0 ? -1 : distProxEstacao;
    estacao->codLinhaIntegra = codLinhaIntegra == 0 ? -1 : codLinhaIntegra;
    estacao->codEstacaoIntegra = codEstacaoIntegra == 0 ? -1 : codEstacaoIntegra;

    if(estacao->nomeEstacao != NULL) {
        free(estacao->nomeEstacao);
        estacao->nomeEstacao = NULL;
    }
    if(nomeEstacao != NULL) {
        estacao->tamNomeEstacao = strlen(nomeEstacao);
        estacao->nomeEstacao = strdup(nomeEstacao);
    } else {
        estacao->tamNomeEstacao = 0;
    }

    if(estacao->nomeLinha != NULL) {
        free(estacao->nomeLinha);
        estacao->nomeLinha = NULL;
    }
    if(nomeLinha != NULL) {
        estacao->tamNomeLinha = strlen(nomeLinha);
        estacao->nomeLinha = strdup(nomeLinha);
    } else {
        estacao->tamNomeLinha = 0;
    }
}

char *criar_buffer()
{
    char *buffer = (char *)malloc(sizeof(char) * TAM_REGISTRO);

    return buffer;
}

int escrever_estacao_no_buffer(Estacao *estacao, char *buffer)
{
    // escrever
    int offset = 0;

    memset(buffer, CHAR_LIXO, TAM_REGISTRO);

    memcpy(buffer + offset, &estacao->removido, sizeof(estacao->removido));
    offset += sizeof(estacao->removido);
    memcpy(buffer + offset, &estacao->proximo, sizeof(estacao->proximo));
    offset += sizeof(estacao->proximo);
    memcpy(buffer + offset, &estacao->codEstacao, sizeof(estacao->codEstacao));
    offset += sizeof(estacao->codEstacao);
    memcpy(buffer + offset, &estacao->codLinha, sizeof(estacao->codLinha));
    offset += sizeof(estacao->codLinha);
    memcpy(buffer + offset, &estacao->codProxEstacao, sizeof(estacao->codProxEstacao));
    offset += sizeof(estacao->codProxEstacao);
    memcpy(buffer + offset, &estacao->distProxEstacao, sizeof(estacao->distProxEstacao));
    offset += sizeof(estacao->distProxEstacao);
    memcpy(buffer + offset, &estacao->codLinhaIntegra, sizeof(estacao->codLinhaIntegra));
    offset += sizeof(estacao->codLinhaIntegra);
    memcpy(buffer + offset, &estacao->codEstacaoIntegra, sizeof(estacao->codEstacaoIntegra));
    offset += sizeof(estacao->codEstacaoIntegra);
    memcpy(buffer + offset, &estacao->tamNomeEstacao, sizeof(estacao->tamNomeEstacao));
    offset += sizeof(estacao->tamNomeEstacao);
    memcpy(buffer + offset, estacao->nomeEstacao, estacao->tamNomeEstacao);
    offset += estacao->tamNomeEstacao;
    memcpy(buffer + offset, &estacao->tamNomeLinha, sizeof(estacao->tamNomeLinha));
    offset += sizeof(estacao->tamNomeLinha);
    memcpy(buffer + offset, estacao->nomeLinha, estacao->tamNomeLinha);
    offset += estacao->tamNomeLinha;

    return offset; // Retorna o número de bytes escritos no buffer
}

int escrever_buffer_na_estacao(char *buffer, Estacao *estacao)
{

    int offset = 0;
    char removido;
    int proximo;
    int codEstacao;
    int codLinha;
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaIntegra;
    int codEstacaoIntegra;
    int tamNomeEstacao;
    int tamNomeLinha;

    memcpy(&removido, buffer + offset, sizeof(removido));
    offset += sizeof(removido);

    // importante para a lógica de limpeza
    estacao->removido = removido;

    memcpy(&proximo, buffer + offset, sizeof(proximo));
    offset += sizeof(proximo);

    estacao->proximo = proximo;

    if (removido == '1')
    {
        return 1; // Registro removido, não preenchemos a estacao, apenas retornamos.
    }

    memcpy(&codEstacao, buffer + offset, sizeof(codEstacao));
    offset += sizeof(codEstacao);

    memcpy(&codLinha, buffer + offset, sizeof(codLinha));
    offset += sizeof(codLinha);

    memcpy(&codProxEstacao, buffer + offset, sizeof(codProxEstacao));
    offset += sizeof(codProxEstacao);

    memcpy(&distProxEstacao, buffer + offset, sizeof(distProxEstacao));
    offset += sizeof(distProxEstacao);

    memcpy(&codLinhaIntegra, buffer + offset, sizeof(codLinhaIntegra));
    offset += sizeof(codLinhaIntegra);

    memcpy(&codEstacaoIntegra, buffer + offset, sizeof(codEstacaoIntegra));
    offset += sizeof(codEstacaoIntegra);

    memcpy(&tamNomeEstacao, buffer + offset, sizeof(tamNomeEstacao));
    offset += sizeof(tamNomeEstacao);

    estacao->nomeEstacao = (char *)malloc(sizeof(char) * (tamNomeEstacao + 1));

    memcpy(estacao->nomeEstacao, buffer + offset, tamNomeEstacao);

    // nomeEstacao[tamNomeEstacao] = '\0';

    offset += tamNomeEstacao;

    memcpy(&tamNomeLinha, buffer + offset, sizeof(tamNomeLinha));
    offset += sizeof(tamNomeLinha);

    char nomeLinha[tamNomeLinha];

    memcpy(nomeLinha, buffer + offset, tamNomeLinha);

    // nomeLinha[tamNomeLinha] = '\0';

    estacao->nomeEstacao[tamNomeEstacao] = '\0';

    estacao->nomeLinha = (char *)malloc(tamNomeLinha + 1);
    memcpy(estacao->nomeLinha, nomeLinha, tamNomeLinha);
    estacao->nomeLinha[tamNomeLinha] = '\0';

    estacao->codEstacao = codEstacao;
    estacao->codLinha = codLinha;
    estacao->codProxEstacao = codProxEstacao;
    estacao->distProxEstacao = distProxEstacao;
    estacao->codLinhaIntegra = codLinhaIntegra;
    estacao->codEstacaoIntegra = codEstacaoIntegra;
    estacao->tamNomeEstacao = tamNomeEstacao;
    estacao->tamNomeLinha = tamNomeLinha;

    return 1;
}



