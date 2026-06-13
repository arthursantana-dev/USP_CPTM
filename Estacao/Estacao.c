#include "Estacao.h"
#include "../EstruturasAuxiliares/EstruturasAuxiliares.h"

/*cria estação normal: campos iniciados com 0 serão convertidos em -1 (nulos)*/
Estacao *criar_estacao(int codEstacao, char *nomeEstacao, int codLinha, char *nomeLinha, int codProxEstacao, int distProxEstacao, int codLinhaIntegra, int codEstacaoIntegra)
{
    Estacao *estacao = (Estacao *)calloc(1, sizeof(Estacao));

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
    estacao->nomeEstacao = nomeEstacao;
    estacao->tamNomeLinha = strlen(nomeLinha);
    estacao->nomeLinha = nomeLinha;

    return estacao;
}

/*cria estação com campos de busca definidos como nulos igual a -1, e 0 para campos que não devem ser considerados para a busca*/
Estacao *criar_estacao_para_busca(int codEstacao, char *nomeEstacao, int codLinha, char *nomeLinha, int codProxEstacao, int distProxEstacao, int codLinhaIntegra, int codEstacaoIntegra)
{
    Estacao *estacao = (Estacao *)calloc(1, sizeof(Estacao));

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

    if (estacao->tamNomeEstacao > 0)
        estacao->nomeEstacao = strdup(nomeEstacao);

    estacao->tamNomeLinha = strlen(nomeLinha);
    if (estacao->tamNomeLinha > 0)
        estacao->nomeLinha = strdup(nomeLinha);

    return estacao;
}

// comparar estações — ea é estação de busca (campos 0 são desconsiderados; -1 são escolhidos para serem nulos)
int comparar_estacoes(Estacao *ea, Estacao *eb)
{
    // printf("codEstacao\n");
    if (ea->codEstacao != -2 && ea->codEstacao != eb->codEstacao)
        return 0;
    // printf("codLinha = %d\n", ea->codLinha);

    if (ea->codLinha != -2 && ea->codLinha != eb->codLinha)
        return 0;
    // printf("codProxEstacao\n");

    if (ea->codProxEstacao != -2 && ea->codProxEstacao != eb->codProxEstacao)
        return 0;
    // printf("distProxEstacao\n");

    if (ea->distProxEstacao != -2 && ea->distProxEstacao != eb->distProxEstacao)
        return 0;
    // printf("codLinhaIntegra\n");

    if (ea->codLinhaIntegra != -2 && ea->codLinhaIntegra != eb->codLinhaIntegra)
        return 0;
    // printf("codEstacaoIntegra\n");

    if (ea->codEstacaoIntegra != -2 && ea->codEstacaoIntegra != eb->codEstacaoIntegra)
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

    // if(ea->tamNomeLinha != 0){
    //     printf("-----------------------------\n");
    //     printf("codEstacao: %d\n", ea->codEstacao);
    //     printf("nomeEstacao: %s\n", ea->nomeEstacao);
    //     printf("codLinha: %d\n", ea->codLinha);
    //     printf("nomeLinha: %s\n", ea->nomeLinha);
    //     printf("codProxEstacao: %d\n", ea->codProxEstacao);
    //     printf("distProxEstacao: %d\n", ea->distProxEstacao);
    //     printf("codLinhaIntegra: %d\n", ea->codLinhaIntegra);
    //     printf("codEstacaoIntegra: %d\n", ea->codEstacaoIntegra);
    // }

    // printf("Encontrou\n");

    return 1;
}

void copiar_estacao(Estacao *estacao, Estacao *novos_valores)
{
    if (novos_valores->codEstacao != -2)
        estacao->codEstacao = novos_valores->codEstacao;

    if (novos_valores->codLinha != -2)
        estacao->codLinha = novos_valores->codLinha;

    if (novos_valores->codProxEstacao != -2)
        estacao->codProxEstacao = novos_valores->codProxEstacao;

    if (novos_valores->distProxEstacao != -2)
        estacao->distProxEstacao = novos_valores->distProxEstacao;

    if (novos_valores->codLinhaIntegra != -2)
        estacao->codLinhaIntegra = novos_valores->codLinhaIntegra;

    if (novos_valores->codEstacaoIntegra != -2)
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

// diferentemente do destruir_estacao, não libera a struct
void limpar_estacao(Estacao *estacao)
{
    if (estacao != NULL)
    {
        if (estacao->nomeEstacao != NULL)
        {
            free(estacao->nomeEstacao);
            estacao->nomeEstacao = NULL;
        }

        if (estacao->nomeLinha != NULL)
        {
            free(estacao->nomeLinha);
            estacao->nomeLinha = NULL;
        }
    }
}

void set_valores_estacao_para_busca(Estacao *estacao)
{
    estacao->codEstacao = -2;
    estacao->codLinha = -2;
    estacao->codProxEstacao = -2;
    estacao->distProxEstacao = -2;
    estacao->codLinhaIntegra = -2;
    estacao->codEstacaoIntegra = -2;
    estacao->tamNomeEstacao = 0;
    if (estacao->nomeEstacao != NULL)
    {
        free(estacao->nomeEstacao);
        estacao->nomeEstacao = NULL;
    }
    estacao->tamNomeLinha = 0;
    if (estacao->nomeLinha != NULL)
    {
        free(estacao->nomeLinha);
        estacao->nomeLinha = NULL;
    }
}

void set_estacao(Estacao *estacao, int codEstacao, char *nomeEstacao, int codLinha, char *nomeLinha, int codProxEstacao, int distProxEstacao, int codLinhaIntegra, int codEstacaoIntegra)
{

    estacao->codEstacao = codEstacao == -2 ? -1 : codEstacao;
    estacao->codLinha = codLinha == -2 ? -1 : codLinha;
    estacao->codProxEstacao = codProxEstacao == -2 ? -1 : codProxEstacao;
    estacao->distProxEstacao = distProxEstacao == -2 ? -1 : distProxEstacao;
    estacao->codLinhaIntegra = codLinhaIntegra == -2 ? -1 : codLinhaIntegra;
    estacao->codEstacaoIntegra = codEstacaoIntegra == -2 ? -1 : codEstacaoIntegra;

    if (estacao->nomeEstacao != NULL)
    {
        free(estacao->nomeEstacao);
        estacao->nomeEstacao = NULL;
    }
    if (nomeEstacao != NULL)
    {
        estacao->tamNomeEstacao = strlen(nomeEstacao);
        estacao->nomeEstacao = strdup(nomeEstacao);
    }
    else
    {
        estacao->tamNomeEstacao = 0;
    }

    if (estacao->nomeLinha != NULL)
    {
        free(estacao->nomeLinha);
        estacao->nomeLinha = NULL;
    }
    if (nomeLinha != NULL)
    {
        estacao->tamNomeLinha = strlen(nomeLinha);
        estacao->nomeLinha = strdup(nomeLinha);
    }
    else
    {
        estacao->tamNomeLinha = 0;
    }
}

/* Buffer */

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

    return offset; // retorna o número de bytes escritos no buffer
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

    // importante para a lógica de limpeza
    estacao->proximo = proximo;

    if (removido == '1')
    {
        return 1; // registro removido, não preenchemos a estacao, apenas retornamos.
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

    return 0;
}

void escrever_buffer_no_arquivo(FILE *f_dados, char *buffer)
{
    fwrite(buffer, sizeof(char), TAM_REGISTRO, f_dados);
}

void atualizar_nros_estacoes_no_header(FILE *f_dados)
{
    Header *header_dados = ler_header_do_arquivo(f_dados);
    header_dados->status = '0';

    escrever_header_no_arquivo(f_dados, header_dados);

    Estacao *estacao = criar_estacao_para_busca(0, "", 0, "", 0, 0, 0, 0);
    SetNomesEstacoes *set_estacoes = criar_set_estacoes();
    InfoParesEstacoes info_pares_estacoes;

    inicializar_pares(&info_pares_estacoes);

    fseek(f_dados, TAM_HEADER, SEEK_SET);

    char buffer[TAM_REGISTRO];

    int numero_estacoes = 0;

    // contagem de número de estações únicas e pares válidos
    while (fread(buffer, TAM_REGISTRO, 1, f_dados) == 1)
    {
        escrever_buffer_na_estacao(buffer, estacao);

        // se estiver logicamente removido, ignora e vai pro próximo
        if (estacao->removido == '1')
        {
            limpar_estacao(estacao);
            continue;
        }

        numero_estacoes++;

        // só inclui e incrementa se a estação ainda não existir no Set
        if (!existe_estacao(set_estacoes, estacao->nomeEstacao))
        {
            incluir_estacao(set_estacoes, estacao->nomeEstacao);
        }

        // só insere o par se existir uma próxima estação válida
        if (estacao->codProxEstacao != -1)
        {
            inserir_par(&info_pares_estacoes, estacao->codEstacao, estacao->codProxEstacao);
        }

        // printf("Qtd Estacoes: %d | Qtd Pares: %d\n", set_estacoes->tamanho, info_pares_estacoes.nroPares);

        limpar_estacao(estacao);
    }

    header_dados->status = '1';
    header_dados->nroEstacoes = set_estacoes->tamanho;
    header_dados->nroParesEstacao = info_pares_estacoes.nroPares;

    escrever_header_no_arquivo(f_dados, header_dados);
    destruir_set_estacoes(set_estacoes);
    destruir_pares(&info_pares_estacoes);

    free(header_dados);

    destruir_estacao(estacao);
}
