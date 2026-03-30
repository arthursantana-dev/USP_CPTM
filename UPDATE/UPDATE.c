#include "UPDATE.h"

// estacao_valores: valores == 0 -> ignorar e manter;
// valores == -1 -> atualizar para NULO
int atualizar_registro(Estacao *estacao_busca, Estacao *estacao_valores, FILE *f)
{

    char buffer[TAM_REGISTRO];

    if (f == NULL)
    {
        mostrar_erro();
        return EXIT_FAILURE;
    }

    Header *header = ler_header_do_arquivo(f);

    header->status = 0;

    if (header == NULL)
    {
        free(header);
        mostrar_erro();
        fclose(f);
        return EXIT_FAILURE;
    }

    int nroEstacoes = header->nroEstacoes;
    int offset = 0;
    int RRNnovo = 0;

    int encontrou_estacao = 0;

    Estacao *ea = (Estacao *)malloc(sizeof(Estacao));

    fseek(f, TAM_HEADER, SEEK_SET);

    for (int i = 0; i < nroEstacoes; i++)
    {

        if (fread(buffer, TAM_REGISTRO, 1, f) != 1)
        {
            mostrar_erro();
            free(header);
            liberar_estacao(ea);
            fclose(f);
            return EXIT_FAILURE;
        }

        escrever_buffer_na_estacao(buffer, ea);

        if (!comparar_estacoes(estacao_busca, ea) || ea->removido == 1)
        {
            liberar_estacao(ea);
            continue;
        }

        encontrou_estacao = 1;

        offset = TAM_HEADER + TAM_REGISTRO * i;
        RRNnovo = (offset - TAM_HEADER) / TAM_REGISTRO;

        printf("Estação encontrada (RRN: %d)\n Estação já foi removida? %s\n", RRNnovo, ea->removido == 1 ? "SIM" : "NÃO");

        break;
    }

    if (!encontrou_estacao)
    {
        free(header);
        liberar_estacao(ea);
        free(ea);
        mostrar_erro();
        return EXIT_FAILURE;
    }

    editar_estacao(ea, estacao_valores);

    escrever_estacao_no_buffer(ea, buffer);

    fseek(f, offset, SEEK_SET);

    escrever_buffer_no_arquivo(f, buffer);

    printf("Estação atualizada com sucesso!\n");

    utils_imprimir_estacao(ea);

    free(header);
    liberar_estacao(ea);
    free(ea);

    return EXIT_SUCCESS;
}
