#include "UPDATE.h"

// estacao_valores: valores == 0 -> ignorar e manter;
// valores == -1 -> atualizar para NULO
int UPDATE(Estacao *estacao_busca, Estacao *estacao_valores, FILE *f)
{

    char buffer[TAM_REGISTRO];

    Header *header = ler_header_do_arquivo(f);

    if (header == NULL)
    {
        free(header);
        mostrar_erro();
        return EXIT_FAILURE;
    }

    header->status = '0';
    escrever_header_no_arquivo(f, header);

    fseek(f, TAM_HEADER, SEEK_SET);

    int i = -1;

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        i++;
        Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));
        escrever_buffer_na_estacao(buffer, ea);

        if (ea->removido == '1')
        {
            destruir_estacao(ea);
            continue;
        }

        if (comparar_estacoes(estacao_busca, ea))
        {

            editar_estacao(ea, estacao_valores);

            escrever_estacao_no_buffer(ea, buffer);

            fseek(f, -TAM_REGISTRO, SEEK_CUR);

            escrever_buffer_no_arquivo(f, buffer);

            fseek(f, 0, SEEK_CUR);
        }

        destruir_estacao(ea);
    }

    header->status = '1';

    fseek(f, 0, SEEK_SET);

    escrever_header_no_arquivo(f, header);

    free(header);

    return EXIT_SUCCESS;
}
