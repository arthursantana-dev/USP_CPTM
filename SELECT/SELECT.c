#include "SELECT.h"

// Façamos o C do CRUD, o celect

struct _query{
    char chave[50];
    char valor[MAX_TAM_NOME];
};

void select_all(FILE *f)
{
    // Abrindo arquivo binário, lendo header, verificações etc
    char buffer[TAM_REGISTRO];
    if (f == NULL)
    {
        mostrar_erro();
        return;
    }
    Header *header = ler_header_do_arquivo(f);
    if (header == NULL)
    {
        mostrar_erro();
        fclose(f);
        return;
    }
    int nroEstacoes = header->nroEstacoes;
    if (nroEstacoes == 0)
    {
        printf("Registro inexistente.\n");
        free(header);
        fclose(f);
        return;
    }
    Estacao *ea = (Estacao *)malloc(sizeof(Estacao));
    fseek(f, TAM_HEADER, SEEK_SET);

    // Imprimir estação se não der erro
    for (int i = 0; i < nroEstacoes; i++)
    {
        if (fread(buffer, TAM_REGISTRO, 1, f) != 1)
        {
            mostrar_erro();
            free(header);
            free(ea);
            fclose(f);
            return;
        }
        escrever_buffer_na_estacao(buffer, ea);
        if (ea->removido == 1)
        {
            continue;
        }
        utils_imprimir_estacao_ln(ea);
    }

    free(header);
    destruir_estacao(ea);
    fclose(f);
}

// Pra ficar bonitinho com select(where()) vou retornar uma lista de wheres
// Lê o mn e itera sobre ele pra fazer a lista de queries
LISTA* where()
{
    int mn;
    scanf("%d", &mn);
    LISTA* wheres = lista_criar();
    for(int i = 0; i < mn; i++)
    {
        QUERY *query = (QUERY*) malloc(sizeof(QUERY));
        scanf("%s", query->chave);
        ScanQuoteString(query->valor, 1);
        lista_inserir(wheres, query);
    }
    return wheres;
}

LISTA* where_interno(int mn, char** chaves, char** valores){
    LISTA* wheres = lista_criar();
    for(int i = 0; i < mn; i++)
    {
        QUERY *query = (QUERY*) malloc(sizeof(QUERY));
        strcpy(query->chave, chaves[i]);
        strcpy(query->valor, valores[i]);
        lista_inserir(wheres, query);
    }
    return wheres;
}

// O select em si, o goat
LISTA* SELECT(LISTA* where, FILE* f)
{
    if (f == NULL)
    {
        mostrar_erro();
        return NULL;
    }
    fseek(f, TAM_HEADER, SEEK_SET);
    LISTA* resultados = lista_criar();
    char *buffer = criar_buffer();
    Header *header = ler_header_do_arquivo(f);
    if (header == NULL)
    {
        mostrar_erro();
        fclose(f);
        return NULL;
    }
    int nroEstacoes = header->nroEstacoes;
    if (nroEstacoes == 0)
    {
        printf("Registro inexistente.\n");
        free(header);
        fclose(f);
        return NULL;
    }
    Estacao *ea = (Estacao *)malloc(sizeof(Estacao));
    fseek(f, TAM_HEADER, SEEK_SET);

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1){
        escrever_buffer_na_estacao(buffer, ea);
        if (ea->removido == 1)
        {
            continue;
        }
        // Como é um AND, um bom jeito de saber se passou por todas as verificações é dar 0 (false) caso falhe em alguma
        bool match = true;
        NO* atual = lista_get_no_head(where);
        if (atual != NULL)
            atual = no_get_anterior(atual); // pula o dummy head
        // Iterando sobre todas as queries
        // atual = query
        while(atual != NULL)
        {
            QUERY* query = (QUERY*) no_get_valor(atual);

            // Checando qual chave estamos procurando
            // Não sei se tem um jeito melhor de fazer isso
            if(strcmp(query->chave, "codEstacao") == 0)
            {
                if(ea->codEstacao != atoi(query->valor))
                {
                    match = false;
                    break;
                }
            }
            
            else if(strcmp(query->chave, "nomeEstacao") == 0)
            {
                if(strcmp(ea->nomeEstacao, query->valor) != 0)
                {
                    match = false;
                    break;
                }
            }

            else if(strcmp(query->chave, "codLinha") == 0)
            {
                if(ea->codLinha != atoi(query->valor))
                {
                    match = false;
                    break;
                }
            }

            else if(strcmp(query->chave, "nomeLinha") == 0)
            {
                if(strcmp(ea->nomeLinha, query->valor) != 0)
                {
                    match = false;
                    break;
                }
            }

            else if(strcmp(query->chave, "codProxEstacao") == 0)
            {
                if(ea->codProxEstacao != atoi(query->valor))
                {
                    match = false;
                    break;
                }
            }

            else if(strcmp(query->chave, "distProxEstacao") == 0)
            {
                if(ea->distProxEstacao != atoi(query->valor))
                {
                    match = false;
                    break;
                }
            }

            else if(strcmp(query->chave, "codLinhaIntegra") == 0)
            {
                if(ea->codLinhaIntegra != atoi(query->valor))
                {
                    match = false;
                    break;
                }
            }

            else if(strcmp(query->chave, "codEstacaoIntegra") == 0)
            {
                if(ea->codEstacaoIntegra != atoi(query->valor))
                {
                    match = false;
                    break;
                }
            }

            atual = no_get_anterior(atual);
        }
        if(match)
        {
            Estacao* resultado = (Estacao*) malloc(sizeof(Estacao));
            *resultado = *ea; // Copia os dados da estação para o resultado
            lista_inserir(resultados, resultado);
        }
    }

    free(header);
    destruir_estacao(ea);
    fclose(f);
    lista_apagar(&where, free);
    return resultados;
}