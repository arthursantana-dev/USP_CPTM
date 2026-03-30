#ifndef NO_H
#define NO_H

typedef struct No NO;

NO* no_criar(void* valor, NO* anterior);

NO* no_get_anterior(NO* no);

void no_set_anterior(NO* no, NO* anterior);

void* no_get_valor(NO* no);

void no_set_valor(NO* no, void* valor);

void no_remover(NO **no);

void no_remover_ponteiro(NO *no);

#endif