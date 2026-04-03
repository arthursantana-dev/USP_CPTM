#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SetNomes.h"

#include "../CONSTS.h"

SetNomesEstacoes *criar_set_estacoes()
{
    SetNomesEstacoes *set = (SetNomesEstacoes *)malloc(sizeof(SetNomesEstacoes));
    if (set == NULL)
        return NULL;

    set->tamanho = 0;
    set->capacidade = MAX_ESTACOES;
    set->nomes = (char **)malloc(set->capacidade * sizeof(char *));

    if (set->nomes == NULL)
    {
        free(set);
        return NULL;
    }

    return set;
}

int existe_estacao(SetNomesEstacoes *set, const char *nome)
{
    if (set == NULL || nome == NULL)
        return 0;

    for (int i = 0; i < set->tamanho; i++)
    {
        if (strcmp(set->nomes[i], nome) == 0)
        {
            return 1; // Encontrou
        }
    }
    return 0; // Não encontrou
}

int incluir_estacao(SetNomesEstacoes *set, const char *nome)
{
    if (set == NULL || nome == NULL)
        return 0;

    if (existe_estacao(set, nome))
    {
        return 0;
    }

    // if (set->tamanho == set->capacidade) {
    //     set->capacidade *= 2;
    //     char** novo_array = (char**)realloc(set->nomes, set->capacidade * sizeof(char*));
    //     if (novo_array == NULL) return 0;
    //     set->nomes = novo_array;
    // }

    set->nomes[set->tamanho] = (char *)malloc((strlen(nome) + 1) * sizeof(char));
    if (set->nomes[set->tamanho] == NULL)
        return 0;

    strcpy(set->nomes[set->tamanho], nome);
    set->tamanho++;

    return 1;
}

int tamanho_set_estacoes(SetNomesEstacoes *set)
{
    if (set == NULL)
        return 0;
    return set->tamanho;
}

int remover_estacao(SetNomesEstacoes *set, const char *nome)
{
    if (set == NULL || nome == NULL)
        return 0;

    // Procura pela estação no array
    for (int i = 0; i < set->tamanho; i++)
    {
        if (strcmp(set->nomes[i], nome) == 0)
        {
            // 1. Encontrou! Libera a memória da string que está sendo removida
            free(set->nomes[i]);

            // 2. Se a estação removida não for a última do array,
            // copiamos o ponteiro da última estação para a posição atual.
            if (i < set->tamanho - 1)
            {
                set->nomes[i] = set->nomes[set->tamanho - 1];
            }

            // 3. Reduzimos o tamanho do set
            set->tamanho--;

            // Opcional: limpa o ponteiro que sobrou no final por segurança
            set->nomes[set->tamanho] = NULL;

            return 1; // Sucesso na remoção
        }
    }

    return 0; // Estação não foi encontrada
}

int destruir_set_estacoes(SetNomesEstacoes *set)
{
    if (set == NULL)
        return 0;

    for (int i = 0; i < set->tamanho; i++)
    {
        free(set->nomes[i]);
        set->nomes[i] = NULL;
    }

    free(set->nomes);
    free(set);        

    return 0;
}

SetNomesEstacoes *criar_set_estacoes_populado(FILE *f)
{
    SetNomesEstacoes *set = criar_set_estacoes();
    if (set == NULL || f == NULL)
        return NULL;

    char *buffer = criar_buffer();
    fseek(f, TAM_HEADER, SEEK_SET);

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        Estacao *ea = (Estacao *)malloc(sizeof(Estacao));
        escrever_buffer_na_estacao(buffer, ea);

        if (ea->removido == '1')
        {
            destruir_estacao(ea);
            continue;
        }

        incluir_estacao(set, ea->nomeEstacao);
        destruir_estacao(ea);
    }

    free(buffer);

    return set;
}   