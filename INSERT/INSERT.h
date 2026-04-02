#ifndef INSERT_H
#define INSERT_H

#include <stdio.h>
#include <stdlib.h>

#include "../utils/utils.h"
#include "../arquivo/arquivo.h"
#include "../buffer/buffer.h"
#include "../ParEstacoes/ParEstacoes.h"
#include "../SetNomes/SetNomes.h"
#include "../SELECT/SELECT.h"

int INSERT(FILE *f);

int _insert(FILE *f, Estacao *estacao);

#endif