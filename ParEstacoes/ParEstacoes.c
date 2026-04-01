#include <stdio.h>
#include <stdlib.h>
#include "ParEstacoes.h"

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

void destruir_pares_estacoes(InfoParesEstacoes *info_pares_estacoes, int codEstacaoRemovida)
{
    ParEstacoes *atual = info_pares_estacoes->inicio;
    ParEstacoes *anterior = NULL;

    while (atual != NULL)
    {
        if (atual->origem == codEstacaoRemovida)
        {
            ParEstacoes *remover = atual;

            if (anterior == NULL)
            {
                info_pares_estacoes->inicio = atual->prox;
            }
            else
            {
                anterior->prox = atual->prox;
            }

            atual = atual->prox;

            printf("\tPar de estações removido (total restando: %d): Origem: %d, Destino: %d\n", 
                   info_pares_estacoes->nroPares - 1, remover->origem, remover->destino);

            free(remover);
            info_pares_estacoes->nroPares--;

            break;
        }
        else
        {
            anterior = atual;
            atual = atual->prox;
        }
    }
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

void ordenar_pares(InfoParesEstacoes *info)
{
    if (info->nroPares <= 1)
    {
        return;
    }

    ParEstacoes *vetor_temp = (ParEstacoes *)malloc(info->nroPares * sizeof(ParEstacoes));
    if (vetor_temp == NULL)
    {
        printf("Erro: Falha na alocação do vetor temporário para ordenação.\n");
        return;
    }

    ParEstacoes *atual = info->inicio;
    for (int i = 0; i < info->nroPares; i++)
    {
        vetor_temp[i].origem = atual->origem;
        vetor_temp[i].destino = atual->destino;
        atual = atual->prox;
    }

    qsort(vetor_temp, info->nroPares, sizeof(ParEstacoes), comparar_pares);

    atual = info->inicio;
    for (int i = 0; i < info->nroPares; i++)
    {
        atual->origem = vetor_temp[i].origem;
        atual->destino = vetor_temp[i].destino;
        atual = atual->prox;
    }

    free(vetor_temp);
}

void iniciar_iterador(InfoParesEstacoes *info, ParEstacoes **par_externo)
{
    if (info == NULL || par_externo == NULL)
    {
        return;
    }

    *par_externo = info->inicio;
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