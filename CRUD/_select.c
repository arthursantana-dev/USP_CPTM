#include "../ParEstacoes/ParEstacoes.h"
#include "../SetNomes/SetNomes.h"

int _select(Estacao *estacao_selecao, FILE *f)
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

    Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        
        escrever_buffer_na_estacao(buffer, ea);

        if (ea->removido == '1')
        {
            limpar_estacao(ea);
            continue;
        }

        if (comparar_estacoes(estacao_selecao, ea))
        {
            utils_imprimir_estacao_ln(ea);
            achou = 1; // Encontrou pelo menos um registro nessa consulta, então não mostramos a mensagem de inexistente.
        }

        limpar_estacao(ea);
    }

    destruir_estacao(ea);

    if (!achou)
    {
        printf("Registro inexistente.\n");
    }

    free(header);

    return EXIT_SUCCESS;
}
