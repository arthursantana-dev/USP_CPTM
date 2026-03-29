#include "SELECT.h"

void select(char *nome_arquivo_binario)
{
    char buffer[TAM_REGISTRO];
    FILE *f = fopen(nome_arquivo_binario, "rb");
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
}