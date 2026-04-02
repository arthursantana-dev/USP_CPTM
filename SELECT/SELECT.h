#ifndef SELECT_H
#define SELECT_H

#include <stdio.h>
#include <stdlib.h>

#include "../utils/utils.h"
#include "../ParEstacoes/ParEstacoes.h"
#include "../buffer/buffer.h"
#include "../Header/Header.h"
#include "../arquivo/arquivo.h"
#include "../lista/lista.h"

typedef struct _query QUERY;
void select_all(FILE *f);
LISTA *where();
LISTA *SELECT(LISTA *where, FILE *f);
LISTA* where_interno(int mn, char** chaves, char** valores);

#endif