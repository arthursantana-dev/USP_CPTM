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
        char nomeEstacao[MAX_TAM_NOME], nomeLinha[MAX_TAM_NOME];
        scanf("%d", &codEstacao);
        ScanQuoteString(nomeEstacao, 1); // nome nulo é "-1" pra ficar mais fácil pro meu select
        ScanQuoteString(codLinha, 2);    // mas eu sei q nomeestacao é não nulo
        ScanQuoteString(nomeLinha, 1);
        ScanQuoteString(codProxEstacao, 2);
        ScanQuoteString(distProxEstacao, 2);
        ScanQuoteString(codLinhaIntegra, 2);
        ScanQuoteString(codEstacaoIntegra, 2);
        Estacao *estacao = criar_estacao(codEstacao, nomeEstacao, codLinha, nomeLinha, codProxEstacao, distProxEstacao, codLinhaIntegra, codEstacaoIntegra);
        if(!_insert(f,estacao)){
            printf("Inserido com sucesso:\n");
            utils_imprimir_estacao_ln(estacao);
            printf("--------------\n");
        }
    }
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
    escrever_estacao_no_buffer(buffer, estacao);
    escrever_buffer_no_arquivo(f, buffer);

    if (estacao->codProxEstacao != -1)
        // Se a estação tem uma próxima estação, então tem um par
        header->nroParesEstacao++;

    // Select from estacoes where codProxEstacao = estacao->codEstacao
    // Para verificar se tem alguma estação apontando pra essa (o que resultaria num novo par)
    char *chave = "codProxEstacao";
    LISTA *estacoes_apontando = SELECT(where_interno(1, &chave, &(estacao->codEstacao)), f);
    header->nroParesEstacao += lista_tamanho(estacoes_apontando);
    lista_apagar(&estacoes_apontando, destruir_estacao);

    // Verificar pra mudar no header o n de estações
    SetNomesEstacoes *set_estacoes = criar_set_estacoes_populado(f);

    // Se a estação já tá no set, então o n de estações continua igual
    if (existe_estacao(set_estacoes, estacao->nomeEstacao))
    {
        free(buffer);
        free(header);
        destruir_estacao(estacao);
        destruir_set_estacoes(set_estacoes);
        return 1;
    }
    header->nroEstacoes++;
    header->status = '1';
    escrever_header_no_arquivo(f, header);

    free(buffer);
    free(header);
    destruir_estacao(estacao);
    destruir_set_estacoes(set_estacoes);

    return 0;
}