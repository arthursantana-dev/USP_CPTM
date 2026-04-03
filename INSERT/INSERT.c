#include "INSERT.h"

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
        if(_insert(f,estacao)){
            mostrar_erro();
        }
    }

    return 0;
}

int _insert(FILE *f, Estacao *estacao)
{
    // Algoritmo da professora
    Header *header = ler_header_do_arquivo(f);
    if (header == NULL)
        return 1;
    header->status = '0';
    escrever_header_no_arquivo(f, header);
    int topo = header->topo;
    int proxRRN = header->proxRRN;
    char *buffer = criar_buffer();
    if (topo == -1)
    {
        int offset = proxRRN * TAM_REGISTRO + TAM_HEADER;
        fseek(f, offset, SEEK_SET);
        header->proxRRN++;
    }
    else
    {
        int offset = topo * TAM_REGISTRO + TAM_HEADER;
        fseek(f, offset, SEEK_SET);
        fread(buffer, TAM_REGISTRO, 1, f);
        fseek(f, offset, SEEK_SET);
        Estacao *estacao_removida = (Estacao *)malloc(sizeof(Estacao));
        escrever_buffer_na_estacao(buffer, estacao_removida);
        header->topo = estacao_removida->proximo;
        destruir_estacao(estacao_removida);
    }
    escrever_estacao_no_buffer(estacao, buffer);
    escrever_buffer_no_arquivo(f, buffer);

    if (estacao->codProxEstacao != -1)
        // Se a estação tem uma próxima estação, então tem um par
        header->nroParesEstacao++;

    // Select from estacoes where codProxEstacao = estacao->codEstacao
    // Para verificar se tem alguma estação apontando pra essa (o que resultaria num novo par)
    char *chave = "codProxEstacao";
    char *valor = (char *)malloc(12 * sizeof(char));
    sprintf(valor, "%d", estacao->codEstacao);
    LISTA *estacoes_apontando = SELECT(where_interno(1, &chave, &valor), f);
    header->nroParesEstacao += lista_tamanho(estacoes_apontando);
    lista_apagar(&estacoes_apontando, (void (*)(void *))destruir_estacao);

    // Verificar pra mudar no header o n de estações
    SetNomesEstacoes *set_estacoes = criar_set_estacoes_populado(f);

    // Se a estação já tá no set, então o n de estações continua igual
    if (!existe_estacao(set_estacoes, estacao->nomeEstacao))
    {
        header->nroEstacoes++;
    }
    
    header->status = '1';
    escrever_header_no_arquivo(f, header);

    free(buffer);
    free(header);
    destruir_estacao(estacao);
    destruir_set_estacoes(set_estacoes);

    return 0;
}