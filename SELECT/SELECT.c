#include "SELECT.h"

// Façamos o C do CRUD, o celect

struct _query
{
    char chave[50];
    char valor[MAX_TAM_NOME];
};

int select_all(FILE *f)
{
    // Abrindo arquivo binário, lendo header, verificações etc
    char buffer[TAM_REGISTRO];
    if (f == NULL)
    {
        return EXIT_FAILURE;
    }
    Header *header = ler_header_do_arquivo(f);
    if (header == NULL)
    {
        return EXIT_FAILURE;
    }
    int nroEstacoes = header->nroEstacoes;
    if (nroEstacoes == 0)
    {
        printf("Registro inexistente.\n");
        free(header);
        return 0; //erro tratado localmente, sem necessidade de flag
    }

    fseek(f, TAM_HEADER, SEEK_SET);

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        Estacao *ea = (Estacao *)malloc(sizeof(Estacao));
        escrever_buffer_na_estacao(buffer, ea);
        if (ea->removido == '1')
        {
            destruir_estacao(ea);
            continue;
        }
        utils_imprimir_estacao_ln(ea);

        destruir_estacao(ea);
    }

    free(header);
    return 0;
}

// Pra ficar bonitinho com select(where()) vou retornar uma lista de wheres
// Lê o mn e itera sobre ele pra fazer a lista de queries
LISTA *where()
{
    int mn;
    scanf("%d", &mn);
    LISTA *wheres = lista_criar();
    for (int i = 0; i < mn; i++)
    {
        QUERY *query = (QUERY *)malloc(sizeof(QUERY));
        scanf("%s", query->chave);
        nullOrString(query->valor);
        lista_inserir(wheres, query);
    }
    return wheres;
}

LISTA *where_interno(int mn, char **chaves, char **valores)
{
    LISTA *wheres = lista_criar();
    for (int i = 0; i < mn; i++)
    {
        QUERY *query = (QUERY *)malloc(sizeof(QUERY));
        strcpy(query->chave, chaves[i]);
        strcpy(query->valor, valores[i]);
        lista_inserir(wheres, query);
    }
    return wheres;
}

// O select em si, o goat
int SELECT(LISTA *where, FILE *f)
{
    if (f == NULL)
    {
        return EXIT_FAILURE;
    }
    fseek(f, TAM_HEADER, SEEK_SET);
    char *buffer = criar_buffer();
    Header *header = ler_header_do_arquivo(f);
    if (header == NULL)
    {
        return EXIT_FAILURE;
    }
    int nroEstacoes = header->nroEstacoes;
    if (nroEstacoes == 0)
    {
        printf("Registro inexistente.\n");
        free(header);
        return EXIT_FAILURE;
    }
    
    fseek(f, TAM_HEADER, SEEK_SET);

    int found = 0;

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        Estacao *ea = (Estacao *)malloc(sizeof(Estacao));
        escrever_buffer_na_estacao(buffer, ea);
        if (ea->removido == '1')
        {
            continue;
        }
        // Como é um AND, um bom jeito de saber se passou por todas as verificações é dar 0 (false) caso falhe em alguma
        bool match = true;
        NO *atual = lista_get_no_head(where);
        if (atual != NULL)
            atual = no_get_anterior(atual); // pula o dummy head
        // Iterando sobre todas as queries
        // atual = query
        while (atual != NULL)
        {
            QUERY *query = (QUERY *)no_get_valor(atual);

            // Checando qual chave estamos procurando
            // Não sei se tem um jeito melhor de fazer isso
            if (strcmp(query->chave, "codEstacao") == 0)
            {
                if (ea->codEstacao != atoi(query->valor))
                {
                    match = false;
                    break;
                }
            }

            else if (strcmp(query->chave, "nomeEstacao") == 0)
            {
                if (strcmp(ea->nomeEstacao, query->valor) != 0)
                {
                    match = false;
                    break;
                }
            }

            else if (strcmp(query->chave, "codLinha") == 0)
            {
                if (ea->codLinha != atoi(query->valor))
                {
                    match = false;
                    break;
                }
            }

            else if (strcmp(query->chave, "nomeLinha") == 0)
            {
                if (strcmp(ea->nomeLinha, query->valor) != 0)
                {
                    match = false;
                    break;
                }
            }

            else if (strcmp(query->chave, "codProxEstacao") == 0)
            {
                if (ea->codProxEstacao != atoi(query->valor))
                {
                    match = false;
                    break;
                }
            }

            else if (strcmp(query->chave, "distProxEstacao") == 0)
            {
                if (ea->distProxEstacao != atoi(query->valor))
                {
                    match = false;
                    break;
                }
            }

            else if (strcmp(query->chave, "codLinhaIntegra") == 0)
            {
                if (ea->codLinhaIntegra != atoi(query->valor))
                {
                    match = false;
                    break;
                }
            }

            else if (strcmp(query->chave, "codEstIntegra") == 0)
            {
                if (ea->codEstacaoIntegra != atoi(query->valor))
                {
                    match = false;
                    break;
                }
            }

            atual = no_get_anterior(atual);
        }
        if (match)
        {
            found=1;
            utils_imprimir_estacao_ln(ea);
        }

        destruir_estacao(ea);
    }

    if(!found){
        printf("Registro inexistente.\n");
    }
    free(buffer);
    free(header);
    lista_apagar(&where, free);
    
    return 0;
}