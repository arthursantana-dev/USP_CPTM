#include <stdio.h>
#include <stdlib.h>
#include "EstruturasAuxiliares.h"

int comparar_pares(const void *a, const void *b)
{
    ParEstacoes *p1 = (ParEstacoes *)a;
    ParEstacoes *p2 = (ParEstacoes *)b;

    if (p1->origem != p2->origem)
    {
        return p1->origem - p2->origem;
    }

    return p1->destino - p2->destino;
}

void inicializar_pares(InfoParesEstacoes *info)
{
    info->nroPares = 0;
    info->inicio = NULL;
}

void inserir_par(InfoParesEstacoes *info, int origem, int destino)
{

    if (existe_par(info, origem, destino) || destino == -1)
    {
        return;
    }

    ParEstacoes *novo = (ParEstacoes *)malloc(sizeof(ParEstacoes));
    if (novo == NULL)
    {
        printf("Erro: Falha na alocação de memória para o novo par.\n");
        return;
    }

    novo->origem = origem;
    novo->destino = destino;

    novo->prox = info->inicio;
    info->inicio = novo;
    
    info->nroPares++;
}

void destruir_pares(InfoParesEstacoes *info)
{
    ParEstacoes *atual = info->inicio;
    ParEstacoes *proximo = NULL;

    while (atual != NULL)
    {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
    }

    info->inicio = NULL;
    info->nroPares = 0;
}

int existe_par(InfoParesEstacoes *info, int origem, int destino)
{
    if (info == NULL) return 0;

    ParEstacoes *atual = info->inicio;

    while (atual != NULL)
    {
        if (atual->origem == origem && atual->destino == destino)
        {
            return 1; // O par já existe
        }
        atual = atual->prox;
    }

    return 0; // O par não existe
}