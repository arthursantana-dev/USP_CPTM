#include "CRUD.h"

typedef struct {
    void *valor;
    int tipo;
} _comparacao;

_comparacao* get_valor_chave(Estacao *estacao, char *campo)
{
    _comparacao *comp = (_comparacao *)malloc(sizeof(_comparacao));
    if (strcmp(campo, "codEstacao") == 0)
    {
        comp->valor = &(estacao->codEstacao);
        comp->tipo = 0;
        return comp;
    }
    else if (strcmp(campo, "nomeEstacao") == 0)
    {
        comp->valor = estacao->nomeEstacao;
        comp->tipo = 1;
        return comp;
    }
    else if (strcmp(campo, "codLinha") == 0)
    {
        comp->valor = &(estacao->codLinha);
        comp->tipo = 0;
        return comp;
    }
    else if (strcmp(campo, "nomeLinha") == 0)
    {
        comp->valor = estacao->nomeLinha;
        comp->tipo = 1;
        return comp;
    }
    else if (strcmp(campo, "codProxEstacao") == 0)
    {
        comp->valor = &(estacao->codProxEstacao);
        comp->tipo = 0;
        return comp;
    }
    else if (strcmp(campo, "distProxEstacao") == 0)
    {
        comp->valor = &(estacao->distProxEstacao);
        comp->tipo = 0;
        return comp;
    }
    else if (strcmp(campo, "codLinhaIntegra") == 0)
    {
        comp->valor = &(estacao->codLinhaIntegra);
        comp->tipo = 0;
        return comp;
    }
    else if (strcmp(campo, "codEstIntegra") == 0)
    {
        comp->valor = &(estacao->codEstacaoIntegra);
        comp->tipo = 0;
        return comp;
    }
    free(comp);
    return NULL;
}

int comparar_valores(_comparacao *A, _comparacao *B)
{
    if (A->tipo == 0)
    {
        return (*(int *)A->valor == *(int *)B->valor);
    }
    else if (A->tipo == 1)
    {
        return (strcmp((char *)A->valor, (char *)B->valor) == 0);
    }
    return 0; // tipos incompatíveis
}

int NESTED_LOOP_JOIN(FILE *f_dados, char *campo_juncao_A, FILE *f_dados_B, char *campo_juncao_B)
{
    // abrindo arquivo binário, lendo header, verificações etc
    char buffer[TAM_REGISTRO];
    if (f_dados == NULL || f_dados_B == NULL)
    {
        return EXIT_FAILURE;
        
    }
    Header *header_dados = ler_header_do_arquivo(f_dados);
    if (header_dados == NULL)
    {
        
        return EXIT_FAILURE;
    }
    int nroEstacoes = header_dados->nroEstacoes;
    if (nroEstacoes == 0)
    {
        printf("Registro inexistente.\n");
        free(header_dados);
        return 0; // erro tratado localmente, sem necessidade de flag
    }

    fseek(f_dados, TAM_HEADER, SEEK_SET);

    Estacao *estacao_a = (Estacao *)calloc(1, sizeof(Estacao));
    Estacao *estacao_b = (Estacao *)calloc(1, sizeof(Estacao));

    int encontrado = 0;

    // iteração no arquivo e impressão de todas as estações
    while (fread(buffer, TAM_REGISTRO, 1, f_dados) == 1)
    {
        escrever_buffer_na_estacao(buffer, estacao_a);
        if (estacao_a->removido == '1')
        {
            limpar_estacao(estacao_a);
            continue;
        }

        fseek(f_dados_B, TAM_HEADER, SEEK_SET);
        
        while(fread(buffer, TAM_REGISTRO, 1, f_dados_B) == 1)
        {
            escrever_buffer_na_estacao(buffer, estacao_b);
            if (estacao_b->removido == '1')
            {
                limpar_estacao(estacao_b);
                continue;
            }

            _comparacao *compA = get_valor_chave(estacao_a, campo_juncao_A);
            _comparacao *compB = get_valor_chave(estacao_b, campo_juncao_B);

            if (compA == NULL || compB == NULL)
            {
                
                free(compA);
                free(compB);
                destruir_estacao(estacao_a);
                destruir_estacao(estacao_b);
                return EXIT_FAILURE;
            }

            if (comparar_valores(compA, compB))
            {
                encontrado = 1;
                imprimir_join_estacoes(estacao_a, estacao_b);
            }

            free(compA);
            free(compB);

            limpar_estacao(estacao_b);
        }
        limpar_estacao(estacao_a);
    }

    destruir_estacao(estacao_a);
    
    if(!encontrado)
    {
        printf("Registro inexistente.\n");
    }

    free(header_dados);
    return 0;
}

int SINGLE_LOOP_JOIN(FILE *f_dados, FILE *f_dados_B, FILE *f_ab)
{
    // abrindo arquivo binário, lendo header, verificações etc
    char buffer[TAM_REGISTRO];
    if (f_dados == NULL || f_dados_B == NULL || f_ab == NULL)
    {   
        
        return EXIT_FAILURE;
    }
    Header *header_dados = ler_header_do_arquivo(f_dados);
    header_btree header_b = btree_ler_cabecalho(f_ab);
    if (header_dados == NULL || header_b.status == '0')
    {
        
        free(header_dados);
        return EXIT_FAILURE;
    }
    int nroEstacoes = header_dados->nroEstacoes;
    if (nroEstacoes == 0)
    {
        printf("Registro inexistente.\n");
        free(header_dados);
        return 0;
    }

    fseek(f_dados, TAM_HEADER, SEEK_SET);

    Estacao *estacao_a = (Estacao *)calloc(1, sizeof(Estacao));
    Estacao *estacao_b = (Estacao *)calloc(1, sizeof(Estacao));

    int encontrado = 0;

    // iteração no arquivo e impressão de todas as estações
    while (fread(buffer, TAM_REGISTRO, 1, f_dados) == 1)
    {
        escrever_buffer_na_estacao(buffer, estacao_a);
        if (estacao_a->removido == '1')
        {
            limpar_estacao(estacao_a);
            continue;
        }

        int codEstacao = estacao_a->codProxEstacao;
        if (codEstacao == -1)
        {
            limpar_estacao(estacao_a);
            continue;
        }
        int byteoffset = btree_buscar(f_ab, &header_b, codEstacao);
        if (byteoffset == -1)
        {
            limpar_estacao(estacao_a);
            continue;
        }
        fseek(f_dados_B, byteoffset, SEEK_SET);
        fread(buffer, TAM_REGISTRO, 1, f_dados_B);
        escrever_buffer_na_estacao(buffer, estacao_b);
        if (estacao_b->removido == '1')
        {
            limpar_estacao(estacao_b);
            limpar_estacao(estacao_a);
            continue;
        }
        encontrado = 1;
        imprimir_join_estacoes(estacao_a, estacao_b);
        limpar_estacao(estacao_b);
        limpar_estacao(estacao_a);
    }

    destruir_estacao(estacao_a);  
    destruir_estacao(estacao_b);

    if(!encontrado)
    {
        printf("Registro inexistente.\n");
    }

    free(header_dados);
    return 0; 
}