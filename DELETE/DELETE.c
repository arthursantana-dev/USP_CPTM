#include "DELETE.h"

int deletar_registro(char *nome_arquivo_binario, Estacao *estacao_busca, FILE *f)
{

    char buffer[TAM_REGISTRO];

    int removeu_estacao = 0;

    // FILE *f = fopen(nome_arquivo_binario, "rb+");

    if (f == NULL)
    {
        mostrar_erro();
        return EXIT_FAILURE;
    }

    Header *header = ler_header_do_arquivo(f);

    header->status = 0;

    if (header == NULL)
    {
        mostrar_erro();
        fclose(f);
        return EXIT_FAILURE;
    }

    printf("n reg: %d\n", header->nroEstacoes);

    int nroEstacoes = header->nroEstacoes;
    int offset = 0;
    int RRNnovo = 0;

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
            return EXIT_FAILURE;
        }

        escrever_buffer_na_estacao(buffer, ea);

        if (!comparar_estacoes(estacao_busca, ea) || ea->removido == 1)
        {
            continue;
        }

        removeu_estacao = 1;

        offset = TAM_HEADER + TAM_REGISTRO * i;
        RRNnovo = (offset - TAM_HEADER) / TAM_REGISTRO;

        printf("Estação encontrada (RRN: %d)\n Estação já foi removida? %s\n", RRNnovo, ea->removido == 1 ? "SIM" : "NÃO");

        break;
    }

    if (!removeu_estacao)
    {
        printf("n reg: %d\n", header->nroEstacoes);

        mostrar_erro();
        return EXIT_FAILURE;
    }

    ea->removido = 1;
    ea->proximo = header->topo;

    header->nroEstacoes--;
    header->topo = RRNnovo;
    header->status = 1;

    printf("n reg: %d\n", header->nroEstacoes);

    escrever_header_no_arquivo(f, header);

    escrever_estacao_no_buffer(ea, buffer);

    fseek(f, TAM_HEADER + TAM_REGISTRO * RRNnovo, SEEK_SET);

    escrever_buffer_no_arquivo(f, buffer);

    printf("Estação %s removida.\n", ea->nomeEstacao);

    return EXIT_SUCCESS;
}