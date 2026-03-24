#include "buffer.h"

char *criar_buffer()
{
    char *buffer = (char *)malloc(sizeof(char) * TAM_REGISTRO);

    return buffer;
}

int escrever_estacao_no_buffer(Estacao *estacao, char *buffer)
{
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
    memcpy(buffer + offset, &estacao->codEstIntegra, sizeof(estacao->codEstIntegra));
    offset += sizeof(estacao->codEstIntegra);
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
    for (int i = 0; i < TAM_REGISTRO; i++)
    {
        printf("%x ", buffer[i]);
    }

    int offset = 0;
    char removido;
    int proximo;
    int codEstacao;
    int codLinha;
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaIntegra;
    int codEstIntegra;
    int tamNomeEstacao;
    int tamNomeLinha;

    memcpy(&removido, buffer + offset, sizeof(removido));
    offset += sizeof(removido);

    memcpy(&proximo, buffer + offset, sizeof(proximo));
    offset += sizeof(proximo);

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

    memcpy(&codEstIntegra, buffer + offset, sizeof(codEstIntegra));
    offset += sizeof(codEstIntegra);

    memcpy(&tamNomeEstacao, buffer + offset, sizeof(tamNomeEstacao));
    offset += sizeof(tamNomeEstacao);

    estacao->nomeEstacao = (char *)malloc(sizeof(char) * (tamNomeEstacao + 1));

    char nomeEstacao[tamNomeEstacao];

    memcpy(nomeEstacao, buffer + offset, tamNomeEstacao);

    // nomeEstacao[tamNomeEstacao] = '\0';

    offset += tamNomeEstacao;

    memcpy(&tamNomeLinha, buffer + offset, sizeof(tamNomeLinha));
    offset += sizeof(tamNomeLinha);

    char nomeLinha[tamNomeLinha];

    memcpy(nomeLinha, buffer + offset, tamNomeLinha);

    // nomeLinha[tamNomeLinha] = '\0';

    estacao->nomeEstacao = (char*)malloc(tamNomeEstacao + 1);
    memcpy(estacao->nomeEstacao, nomeEstacao, tamNomeEstacao);
    estacao->nomeEstacao[tamNomeEstacao] = '\0';

    estacao->nomeLinha = (char*)malloc(tamNomeLinha + 1);
    memcpy(estacao->nomeLinha, nomeLinha, tamNomeLinha);
    estacao->nomeLinha[tamNomeLinha] = '\0';

    estacao->removido = removido;
    estacao->proximo = proximo;
    estacao->codEstacao = codEstacao;
    estacao->codLinha = codLinha;
    estacao->codProxEstacao = codProxEstacao;
    estacao->distProxEstacao = distProxEstacao;
    estacao->codLinhaIntegra = codLinhaIntegra;
    estacao->codEstIntegra = codEstIntegra;
    estacao->tamNomeEstacao = tamNomeEstacao;
    estacao->tamNomeLinha = tamNomeLinha;

    return 1;
}
