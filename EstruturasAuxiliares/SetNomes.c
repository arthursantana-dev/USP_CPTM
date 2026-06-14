#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EstruturasAuxiliares.h"

#include "../CONSTS.h"

SetNomesEstacoes *criar_set_estacoes()
{
    SetNomesEstacoes *set = (SetNomesEstacoes *)malloc(sizeof(SetNomesEstacoes));
    if (set == NULL)
        return NULL;

    set->inicio = NULL;
    set->tamanho = 0;

    return set;
}

int existe_estacao(SetNomesEstacoes *set, const char *nome)
{
    if (set == NULL || nome == NULL)
        return 0;

    NoSet *atual = set->inicio;
    
    while (atual != NULL)
    {
        if (strcmp(atual->nome, nome) == 0)
        {
            return 1;
        }
        atual = atual->prox;
    }
    
    return 0;
}

int incluir_estacao(SetNomesEstacoes *set, const char *nome)
{
    if (set == NULL || nome == NULL)
        return 0;

    if (existe_estacao(set, nome))
    {
        return 0;
    }

    NoSet *novo_no = (NoSet *)malloc(sizeof(NoSet));
    if (novo_no == NULL)
        return 0;

    novo_no->nome = (char *)malloc((strlen(nome) + 1) * sizeof(char));
    if (novo_no->nome == NULL)
    {
        free(novo_no);
        return 0;
    }

    strcpy(novo_no->nome, nome);

    novo_no->prox = set->inicio;
    set->inicio = novo_no;
    
    set->tamanho++;

    return 1;
}

int destruir_set_estacoes(SetNomesEstacoes *set)
{
    if (set == NULL)
        return 0;

    NoSet *atual = set->inicio;
    NoSet *proximo;

    while (atual != NULL)
    {
        proximo = atual->prox;
        free(atual->nome);
        free(atual);
        atual = proximo;
    }

    free(set);        

    return 0;
}