#include "SELECT.h"

// Façamos o C do CRUD, o celect

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
