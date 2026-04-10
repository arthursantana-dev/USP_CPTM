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
        return 0; // erro tratado localmente, sem necessidade de flag
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

int SELECT(Estacao *estacao_selecao, FILE *f)
{

    char buffer[TAM_REGISTRO];

    Header *header = ler_header_do_arquivo(f);

    if (header == NULL)
    {
        free(header);
        return EXIT_FAILURE;
    }

    int nroEstacoes = header->nroEstacoes;
    if (nroEstacoes == 0)
    {
        printf("Registro inexistente.\n");
        free(header);
        return EXIT_SUCCESS;
    }

    fseek(f, TAM_HEADER, SEEK_SET);

    int achou = 0;

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));
        escrever_buffer_na_estacao(buffer, ea);

        if (ea->removido == '1')
        {
            destruir_estacao(ea);
            continue;
        }

        if (comparar_estacoes(estacao_selecao, ea))
        {
            utils_imprimir_estacao_ln(ea);
            achou = 1; // Encontrou pelo menos um registro nessa consulta, então não mostramos a mensagem de inexistente.
        }

        destruir_estacao(ea);
    }

    if (!achou)
    {
        printf("Registro inexistente.\n");
    }

    free(header);

    return EXIT_SUCCESS;
}
