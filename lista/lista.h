
#ifndef LISTA_H
#define LISTA_H

#include "../no/no.h"
#include <stdbool.h>

typedef struct lista_ LISTA;

LISTA *lista_criar();
bool lista_inserir(LISTA *lista, void *valor);
void *lista_remover(LISTA *lista, void *chave, void *(*get_valor)(void *), void (*remover)(void *));
bool lista_apagar(LISTA **lista, void (*remover)(void **));
void *lista_busca(LISTA *lista, int chave, int (*get_valor)(void *));
int lista_tamanho(LISTA *lista);
bool lista_vazia(LISTA *lista);
bool lista_cheia(LISTA *lista);
void lista_imprimir(LISTA *lista, void* (*imprimir_valor)(void*));
NO* lista_get_no_head(LISTA *lista);
NO* lista_get_no_head(LISTA* lista);

#endif

