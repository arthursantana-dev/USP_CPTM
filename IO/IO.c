#include "IO.h"

#include <string.h>
#include <stdlib.h>

#include "../CONSTS.h"
#include "../Estacao/Estacao.h"
#include "../EstruturasAuxiliares/EstruturasAuxiliares.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../Header/Header.h"

/*Escrita*/

/*Leitura*/


// quebra a linha em um vetor de strings
int decompor_linha(char *linha, char *vetor[])
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

// nomeEstacao e nomeLinha alocados "na hora"
void vetor_char_para_estacao(Estacao *estacao, char *vetor[], int num_campos)
{
    int i = 0;

    if (isdigit(vetor[0][0]))
    {
        i++;
    }

    for (; i < num_campos; i++)
    {
        if (strcmp(vetor[i], "codEstacao") == 0)
        {
            estacao->codEstacao = atoi(vetor[i + 1]);
        }
        else if (strcmp(vetor[i], "nomeEstacao") == 0)
        {
            if (estacao->nomeEstacao != NULL)
            {
                free(estacao->nomeEstacao);
            }
            estacao->tamNomeEstacao = strlen(vetor[i + 1]);
            estacao->nomeEstacao = (char *)malloc((estacao->tamNomeEstacao + 1) * sizeof(char));

            strcpy(estacao->nomeEstacao, vetor[i + 1]);
        }
        else if (strcmp(vetor[i], "codLinha") == 0)
        {
            estacao->codLinha = atoi(vetor[i + 1]);
        }
        else if (strcmp(vetor[i], "nomeLinha") == 0)
        {
            if (estacao->nomeLinha != NULL)
            {
                free(estacao->nomeLinha); // Libera o valor anterior ("")
            }
            estacao->tamNomeLinha = strlen(vetor[i + 1]);
            estacao->nomeLinha = (char *)malloc((estacao->tamNomeLinha + 1) * sizeof(char));
            strcpy(estacao->nomeLinha, vetor[i + 1]);
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
            estacao->codEstacaoIntegra = atoi(vetor[i + 1]);
        }
    }
}

void imprimir_estacao(Estacao *estacao)
{
    printf("%d ", estacao->codEstacao);

    if (estacao->nomeEstacao != NULL && estacao->tamNomeEstacao > 0)
        printf("%s ", estacao->nomeEstacao);
    else
        printf("NULO ");

    if (estacao->codLinha != -1)
        printf("%d ", estacao->codLinha);
    else
        printf("NULO ");

    if (estacao->nomeLinha != NULL && estacao->tamNomeLinha > 0)
        printf("%s ", estacao->nomeLinha);
    else
        printf("NULO ");

    if (estacao->codProxEstacao != -1)
        printf("%d ", estacao->codProxEstacao);
    else
        printf("NULO ");

    if (estacao->distProxEstacao != -1)
        printf("%d ", estacao->distProxEstacao);
    else
        printf("NULO ");

    if (estacao->codLinhaIntegra != -1)
        printf("%d ", estacao->codLinhaIntegra);
    else
        printf("NULO ");

    if (estacao->codEstacaoIntegra != -1)
        printf("%d ", estacao->codEstacaoIntegra);
    else
        printf("NULO ");

    printf("\n");
}
