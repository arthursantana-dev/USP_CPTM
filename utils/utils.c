#include "utils.h"

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
    printf("Removido? %s\n", estacao->removido ? "SIM" : "NÃO");
    printf("RRN do próximo removido: %d\n", estacao->proximo);
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
        if (fread(&byte, sizeof(byte), 1, f) != 1)
        {
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

// quebra a linha em um vetor de strings
int utils_decompor_linha(char *linha, char *vetor[])
{
    int contador = 0;
    char *ptr = linha;

    while (*ptr != '\0')
    {
        while (isspace((unsigned char)*ptr))
        {
            ptr++;
        }

        if (*ptr == '\0')
            break;

        if (*ptr == '"')
        {
            ptr++;
            vetor[contador++] = ptr;

            while (*ptr != '"' && *ptr != '\0')
            {
                ptr++;
            }

            if (*ptr == '"')
            {
                *ptr = '\0';
                ptr++;
            }
        }
        else
        {
            vetor[contador++] = ptr;

            while (!isspace((unsigned char)*ptr) && *ptr != '\0')
            {
                ptr++;
            }

            if (*ptr != '\0')
            {
                *ptr = '\0';
                ptr++;
            }
        }
    }

    return contador;
}

void utils_vetor_para_estacao(Estacao *estacao, char *vetor[], int num_campos)
{

    for (int i = 0; i < num_campos; i++)
    {
        if (strcmp(vetor[i], "codEstacao") == 0)
        {
            estacao->codEstacao = atoi(vetor[i + 1]);
        }
        else if (strcmp(vetor[i], "nomeEstacao") == 0)
        {
            estacao->nomeEstacao = vetor[i + 1];
        }
        else if (strcmp(vetor[i], "codLinha") == 0)
        {
            estacao->codLinha = atoi(vetor[i + 1]);
        }
        else if (strcmp(vetor[i], "nomeLinha") == 0)
        {
            estacao->nomeLinha = vetor[i + 1];
        }
        else if (strcmp(vetor[i], "codProxEstacao") == 0)
        {
            estacao->codProxEstacao = atoi(vetor[i + 1]);
        }
        else if (strcmp(vetor[i], "distProxEstacao") == 0)
        {
            estacao->distProxEstacao = atoi(vetor[i + 1]);
        }
        else if (strcmp(vetor[i], "codLinhaIntegra") == 0)
        {
            estacao->codLinhaIntegra = atoi(vetor[i + 1]);
        }
        else if (strcmp(vetor[i], "codEstIntegra") == 0)
        {
            estacao->codEstIntegra = atoi(vetor[i + 1]);
        }
    }
}

void utils_linha_para_estacao(Estacao *estacao, char *linha)
{
    char *elementos[MAX_TOKENS];

    int qtd_elementos = utils_decompor_linha(linha, elementos);

    utils_vetor_para_estacao(estacao, elementos, qtd_elementos);
}

void utils_mostrar_pilha_remocao(FILE *f, Header *header)
{
    int rrn = header->topo;

    char buffer[TAM_REGISTRO];

    while (rrn != -1)
    {
        printf("rrn: %d\n", rrn);
        printf("TAM_HEADER + TAM_REGISTRO * rrn: %d\n", TAM_HEADER + TAM_REGISTRO * rrn);

        Estacao *eb = (Estacao *)malloc(sizeof(Estacao));

        fseek(f, TAM_HEADER + TAM_REGISTRO * rrn, SEEK_SET);

        if (fread(buffer, TAM_REGISTRO, 1, f) != 1)
        {
            mostrar_erro();
            free(header);
            free(eb->nomeEstacao);
            free(eb->nomeLinha);
            free(eb);
            return EXIT_FAILURE;
        }

        escrever_buffer_na_estacao(buffer, eb);

        utils_imprimir_estacao(eb);

        printf("--------------\n");

        rrn = eb->proximo;

        free(eb->nomeEstacao);
        free(eb->nomeLinha);
        free(eb);

        // if(j > 3) break;

        // j++;
    }
}