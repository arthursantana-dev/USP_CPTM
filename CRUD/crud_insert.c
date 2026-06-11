#include "CRUD.h"

int crud_insert(FILE *f, Estacao *estacao, FILE *fab)
{
    Header *header = ler_header_do_arquivo(f);
    if (header == NULL){
        mostrar_erro();
        return 1;
    }

    header_arvore_b header_b = arvore_b_ler_cabecalho(fab);

    // Header inconsistente
    header->status = '0';
    escrever_header_no_arquivo(f, header);

    int topo = header->topo; // RRN do topo da pilha de removidos
    int proxRRN = header->proxRRN; // Próximo RRN disponível
    
    char *buffer = criar_buffer();
    int offset = 0; // Guardando o RRN para usar na Árvore B

    // Se a pilha de removidos estiver vazia, insere no final
    if (topo == -1)
    {
        offset = proxRRN * TAM_REGISTRO + TAM_HEADER;
        fseek(f, offset, SEEK_SET);
        header->proxRRN++;
    }
    else
    {
        // fseek para o topo da pilha de removidos e leitura (para pegar o próximo da pilha)
        offset = topo * TAM_REGISTRO + TAM_HEADER;
        fseek(f, offset, SEEK_SET);
        fread(buffer, TAM_REGISTRO, 1, f);
        fseek(f, offset, SEEK_SET);

        // Pegando o próximo da pilha de removidos
        Estacao *estacao_removida = (Estacao *)calloc(1, sizeof(Estacao));
        escrever_buffer_na_estacao(buffer, estacao_removida);
        header->topo = estacao_removida->proximo;
        destruir_estacao(estacao_removida);
    }
    escrever_estacao_no_buffer(estacao, buffer);

    // Inserção
    escrever_buffer_no_arquivo(f, buffer);
    arvore_b_inserir(fab, &header_b, estacao->codEstacao, offset);


    // if (estacao->codProxEstacao != -1)
    //     // Se a estação tem uma próxima estação, então tem um par
    //     header->nroParesEstacao++;

    header->status = '1';
    escrever_header_no_arquivo(f, header);

    free(buffer);
    free(header);

    return 0;
}

int INSERT(FILE *f, FILE *fab)
{
    if (f == NULL || fab == NULL)
        return 1;

    int n;
    scanf("%d", &n);

    Estacao *estacao = criar_estacao_para_busca(-2, "", -2, "", -2, -2, -2, -2);

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

        set_estacao(estacao, codEstacao, nomeEstacao, codLinha, nomeLinha, codProxEstacao, distProxEstacao, codLinhaIntegra, codEstacaoIntegra);

        free(nomeEstacao);
        free(nomeLinha);

        int erro = crud_insert(f, estacao, fab);
        if (erro)
        {
            destruir_estacao(estacao);
            return EXIT_FAILURE;
        }
        
        limpar_estacao(estacao);
    }

    destruir_estacao(estacao);

    return 0;
}


