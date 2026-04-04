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

/**
 * @brief Gerencia o laço de leitura de entradas do usuário e orquestra a inserção de múltiplos registros novos no arquivo binário.
 * @param f Ponteiro para o arquivo binário aberto no modo de atualização.
 * @return Retorna 0 em caso de sucesso total ou 1 em caso de falha (ex: arquivo nulo).
 */
int INSERT(FILE *f);

/**
 * @brief Insere fisicamente um único registro no arquivo binário, reaproveitando espaços da pilha de registros logicamente removidos quando possível.
 * @param f Ponteiro para o arquivo binário aberto.
 * @param estacao Estrutura populada com os dados do registro que será escrito.
 * @return Retorna 0 em caso de sucesso ou 1 em caso de falha de manipulação.
 */
int _insert(FILE *f, Estacao *estacao);

#endif