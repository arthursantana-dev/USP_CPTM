#include "DELETE.h"

int deletar_registro(char *nome_arquivo_binario, Estacao *estacao_busca)
{

    char buffer[TAM_REGISTRO];

    FILE *f = fopen(nome_arquivo_binario, "rb+");

    if (f == NULL)
    {
        mostrar_erro();
        return EXIT_FAILURE;
    }

    Header *header = ler_header_do_arquivo(f);

    if (header == NULL)
    {
        mostrar_erro();
        fclose(f);
        return EXIT_FAILURE;
    }

    printf("n reg: %d\n", header->nroEstacoes);

    int topo = header->topo;

    Estacao *ea = (Estacao *)malloc(sizeof(Estacao));

    fseek(f, TAM_HEADER, SEEK_SET);

    fseek(f, TAM_REGISTRO * 0, SEEK_CUR);

    fread(buffer, TAM_REGISTRO, 1, f);

    // memcpy(ea, buffer, TAM_REGISTRO);

    escrever_buffer_na_estacao(buffer, ea);

    utils_imprimir_estacao(ea);

    printf("Tamanho nome estação: %d\n", ea->tamNomeEstacao);
    printf("Tamanho nome linha: %d\n", ea->tamNomeLinha);

    // fread(ea, TAM_REGISTRO, 1, f);

    // utils_mostrar_buffer_como_bytes(buffer);

    // printf("%d\n\n", ea->codLinha);

    // utils_imprimir_estacao(ea);

    // utils_mostrar_bytes_do_arquivo(f, 17+80);
}