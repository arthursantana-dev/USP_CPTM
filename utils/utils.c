#include <stdio.h>

#include "../CONSTS.h"
#include "../estacao/estacao.h"

void mostrar_erro()
{
    printf("Falha no processamento do arquivo.\n");
}

void utils_mostrar_buffer_como_bytes(char *buffer)
{
    for (int i = 0; i < TAM_REGISTRO; i++)
    {
        printf("%02X ", (unsigned char)buffer[i]);
        if ((i + 1) % 4 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}

void utils_imprimir_estacao(Estacao *estacao)
{
    printf("Código da estação: %d\n", estacao->codEstacao);
    printf("Nome da estação: %s\n", estacao->nomeEstacao);
    printf("Código da linha: %d\n", estacao->codLinha);
    printf("Nome da linha: %s\n", estacao->nomeLinha);
    printf("Código da próxima estação: %d\n", estacao->codProxEstacao);
    printf("Distância para a próxima estação: %d\n", estacao->distProxEstacao);
    printf("Código da linha de integração: %d\n", estacao->codLinhaIntegra);
    printf("Código da estação de integração: %d\n", estacao->codEstIntegra);
}

void utils_mostrar_bytes_do_arquivo(FILE *f, int num_bytes)
{
    fseek(f, 0, SEEK_SET);
    unsigned char byte;
    for (int i = 0; i < num_bytes; i++)
    {
        if(fread(&byte, sizeof(byte), 1, f) != 1){
            mostrar_erro();
            return;
        }
        
        printf("%02X ", byte);
        if ((i + 1) % 4 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}