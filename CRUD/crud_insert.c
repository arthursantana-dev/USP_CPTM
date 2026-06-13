#include "CRUD.h"

int crud_insert(FILE *f_dados, Estacao *estacao, FILE *f_ab, header_btree *header_b)
{
    Header *header_dados = ler_header_do_arquivo(f_dados);
    if (header_dados == NULL){
        mostrar_erro();
        return 1;
    }

    // impede a inserção de chaves primárias duplicadas checando o índice primeiro
    if (btree_buscar(f_ab, header_b, estacao->codEstacao) != -1)
    {
        free(header_dados);
        return 0; 
    }

    // marca os arquivos de dados e de índice como inconsistentes durante a transação
    header_dados->status = '0';
    escrever_header_no_arquivo(f_dados, header_dados);

    header_b->status = '0';
    btree_atualizar_cabecalho(f_ab, header_b);

    int topo = header_dados->topo; 
    int proxRRN = header_dados->proxRRN; 
    
    char *buffer = criar_buffer();
    int offset = 0; 

    // verifica se não há registros removidos para reaproveitar
    if (topo == -1)
    {
        // calcula o offset físico no final do arquivo e incrementa o contador
        offset = proxRRN * TAM_REGISTRO + TAM_HEADER;
        fseek(f_dados, offset, SEEK_SET);
        header_dados->proxRRN++;
    }
    else
    {
        // salta para o topo da pilha de removidos para ler o encadeamento
        offset = topo * TAM_REGISTRO + TAM_HEADER;
        fseek(f_dados, offset, SEEK_SET);
        fread(buffer, TAM_REGISTRO, 1, f_dados);
        
        // retorna o ponteiro para sobrescrever esse mesmo espaço vazio
        fseek(f_dados, offset, SEEK_SET);

        // recupera o rrn do próximo item removido para ser o novo topo da pilha
        Estacao *estacao_removida = (Estacao *)calloc(1, sizeof(Estacao));
        escrever_buffer_na_estacao(buffer, estacao_removida);
        header_dados->topo = estacao_removida->proximo;
        destruir_estacao(estacao_removida);
    }
    
    escrever_estacao_no_buffer(estacao, buffer);

    // grava o registro no disco e registra seu offset exato na árvore b
    escrever_buffer_no_arquivo(f_dados, buffer);
    btree_inserir(f_ab, header_b, estacao->codEstacao, offset);

    // restaura o status de consistência após o sucesso da operação
    header_dados->status = '1';
    escrever_header_no_arquivo(f_dados, header_dados);

    free(buffer);
    free(header_dados);

    return 0;
}

int INSERT(FILE *f_dados, FILE *f_ab)
{
    if (f_dados == NULL || f_ab == NULL)
        return 1;

    int n;
    scanf("%d", &n);

    Estacao *estacao = criar_estacao_para_busca(-2, "", -2, "", -2, -2, -2, -2);
    
    header_btree header_b = btree_ler_cabecalho(f_ab);


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

        int erro = crud_insert(f_dados, estacao, f_ab, &header_b);
        if (erro)
        {
            destruir_estacao(estacao);
            return EXIT_FAILURE;
        }
        
        limpar_estacao(estacao);
    }

    header_b.status = '1';

    atualizar_nros_estacoes_no_header(f_dados);

    btree_atualizar_cabecalho(f_ab, &header_b);

    destruir_estacao(estacao);

    return 0;
}

// insere um novo registro no arquivo de dados lidando exclusivamente com o reaproveitamento de rrn
int crud_insert_sem_indice(FILE *f_dados, Estacao *estacao)
{
    Header *header = ler_header_do_arquivo(f_dados);
    if (header == NULL){
        mostrar_erro();
        return 1;
    }

    // marca os arquivos de dados como inconsistente durante a transacao
    header->status = '0';
    escrever_header_no_arquivo(f_dados, header);

    int topo = header->topo; 
    int proxRRN = header->proxRRN; 
    
    char *buffer = criar_buffer();
    int offset = 0; 

    // verifica se nao ha registros removidos para reaproveitar e insere no fim
    if (topo == -1)
    {
        offset = proxRRN * TAM_REGISTRO + TAM_HEADER;
        fseek(f_dados, offset, SEEK_SET);
        header->proxRRN++;
    }
    else
    {
        // vai ate o topo da pilha de removidos para atualizar a cabeca da lista
        offset = topo * TAM_REGISTRO + TAM_HEADER;
        fseek(f_dados, offset, SEEK_SET);
        fread(buffer, TAM_REGISTRO, 1, f_dados);
        
        fseek(f_dados, offset, SEEK_SET);

        Estacao *estacao_removida = (Estacao *)calloc(1, sizeof(Estacao));
        escrever_buffer_na_estacao(buffer, estacao_removida);
        header->topo = estacao_removida->proximo;
        destruir_estacao(estacao_removida);
    }
    
    escrever_estacao_no_buffer(estacao, buffer);
    escrever_buffer_no_arquivo(f_dados, buffer);

    // restaura o status de consistencia apos o sucesso
    header->status = '1';
    escrever_header_no_arquivo(f_dados, header);

    free(buffer);
    free(header);

    return 0;
}

// gerencia a captura via terminal e a insercao em lote sem espelhar na arvore b
int INSERT_SEM_INDICE(FILE *f_dados)
{
    if (f_dados == NULL)
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
        nullOrString(nomeLinha); 
        nullOrInt(&codProxEstacao);
        nullOrInt(&distProxEstacao);
        nullOrInt(&codLinhaIntegra);
        nullOrInt(&codEstacaoIntegra);

        set_estacao(estacao, codEstacao, nomeEstacao, codLinha, nomeLinha, codProxEstacao, distProxEstacao, codLinhaIntegra, codEstacaoIntegra);

        free(nomeEstacao);
        free(nomeLinha);

        // chama a funcao que lida apenas com o arquivo de dados
        int erro = crud_insert_sem_indice(f_dados, estacao);
        if (erro)
        {
            destruir_estacao(estacao);
            return EXIT_FAILURE;
        }
        
        limpar_estacao(estacao);
    }

	atualizar_nros_estacoes_no_header(f_dados);

    destruir_estacao(estacao);

    return 0;
}


