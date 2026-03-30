
#include "./lista.h"
#include "../no/no.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct lista_
{
    NO *no_head;
    int tamanho;
};

LISTA *lista_criar()
{
    LISTA *lista = (LISTA *)malloc(sizeof(LISTA));
    if (lista == NULL)
    {
        return NULL;
    }
    lista->tamanho = 0;
    lista->no_head = no_criar(NULL, NULL);
    if (lista->no_head == NULL)
    {
        free(lista);
        return NULL;
    }

    return lista;
}

NO* lista_get_no_head(LISTA *lista)
{
    if (lista == NULL)
    {
        return NULL;
    }
    return lista->no_head;
}

// Com o objetivo de manter a ordem, insere no final da lista, O(n)
bool lista_inserir(LISTA *lista, void *valor)
{
    if (lista == NULL || valor == NULL)
    {
        return false;
    }

    NO *novo_no = no_criar(valor, NULL);
    if (novo_no == NULL)
    {
        return false;
    }
    if (lista->no_head == NULL)
    {
        lista->no_head = novo_no;
    }
    else
    {
        NO *atual = lista->no_head;

        // no_set_anterior(atual, novo_no);

        // lista->no_head = novo_no;

        while (no_get_anterior(atual) != NULL)
        {
            atual = no_get_anterior(atual);
        }
        no_set_anterior(atual, novo_no);
    }

    lista->tamanho++;
    return true;
}

void *lista_remover(LISTA *lista, void *chave, void *(*get_valor)(void *), void (*remover)(void *))
{
    if (lista == NULL || lista->tamanho == 0)
    {
        return NULL;
    }

    NO *atual = lista->no_head;
    NO *anterior = NULL;

    while (atual != NULL)
    {
        // printf("valor atual: %d\n", (int)get_valor(no_get_valor(atual)));
        // printf("chave: %d\n", (int)chave);
        if (get_valor(no_get_valor(atual)) == chave)
        {
            // printf("achou\n");
            if (anterior == NULL)
            {
                lista->no_head = no_get_anterior(atual);
            }
            else
            {
                no_set_anterior(anterior, no_get_anterior(atual));
            }

            char* valor = no_get_valor(atual);

            remover(no_get_valor(atual));
            no_remover_ponteiro(atual);
            lista->tamanho--;
            return valor;
        }
        anterior = atual;
        atual = no_get_anterior(atual);
    }

    return NULL;
}

bool lista_apagar(LISTA **lista, void (*remover)(void **))
{
    if (lista == NULL || *lista == NULL)
    {
        return false;
    }

    NO *atual = (*lista)->no_head;
    NO *proximo;

    while (atual != NULL)
    {
        proximo = no_get_anterior(atual);
        remover(no_get_valor(atual));
        no_remover_ponteiro(atual);
        atual = proximo;
    }

    free(*lista);
    *lista = NULL;
    return true;
}

void lista_imprimir(LISTA *lista, void* (*imprimir_valor)(void *))
{
    if (lista == NULL || lista->tamanho == 0)
    {
        printf("Lista vazia ou nula\n");
        return;
    }

    NO *atual = lista->no_head;

    atual = no_get_anterior(atual);


    while (atual != NULL)
    {
        imprimir_valor(no_get_valor(atual));
        atual = no_get_anterior(atual);
    }
}

int lista_tamanho(LISTA *lista)
{
    if (lista == NULL)
    {
        return -1;
    }
    return lista->tamanho;
}

bool lista_vazia(LISTA *lista)
{
    if (lista == NULL)
    {
        return true;
    }
    return lista->tamanho == 0;
}

bool lista_cheia(LISTA *lista)
{

    NO *no = no_criar(NULL, NULL);
    if (no == NULL)
    {
        return true;
    }

    free(no);
    return false;
}

// Usou-se uma função de callback para obter o valor do nó, permitindo flexibilidade na comparação
void *lista_busca(LISTA *lista, int chave, int (*get_valor)(void *))
{
    if (lista == NULL || lista->tamanho == 0)
    {
        return NULL;
    }

    NO *atual = lista->no_head;

    while (atual != NULL)
    {
        // printf("LISTA BUSCA: valor atual: %x\n", atual);
        // printf("LISTA BUSCA: chave: %d\n", chave);
        // printf("Anterior ao nó: %s\n", (char*)get_valor(no_get_anterior(atual)));

        int comparacao = get_valor(no_get_valor(atual)) == chave;
        //printf("comparacao: %d\n", comparacao);
        // int comparacao = 0;

        if (comparacao == 1)
        {
           // printf("Busca: encontrado\n");
            return no_get_valor(atual);
        }


        atual = no_get_anterior(atual);
    }

    // printf("Busca: nao encontrou\n");

    return NULL;
}