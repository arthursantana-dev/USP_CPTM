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
/**
 * @brief Recupera e exibe de forma sequencial os dados de todos os registros existentes no arquivo binário.
 * @param f Ponteiro para o arquivo binário aberto para leitura.
 */
void select_all(FILE *f);

/**
 * @brief Lê a entrada padrão fornecida pelo usuário para construir dinamicamente uma lista de critérios de seleção.
 * @return Retorna um ponteiro para a LISTA criada, contendo as queries de busca.
 */
LISTA *where();

/**
 * @brief Realiza a busca sequencial de registros no arquivo binário que satisfaçam todos os critérios estabelecidos.
 * @param where Ponteiro para a lista contendo os critérios de busca (pares de chave e valor).
 * @param f Ponteiro para o arquivo binário aberto.
 * @return Retorna uma LISTA contendo cópias profundas dos registros (Estacao) que deram "match" na busca.
 */
LISTA *SELECT(LISTA *where, FILE *f);

/**
 * @brief Constrói internamente uma lista de critérios de busca (WHERE) a partir de vetores de chaves e valores passados via código.
 * @param mn Número total de pares (critérios de busca) informados.
 * @param chaves Vetor de strings contendo os nomes dos campos a serem testados.
 * @param valores Vetor de strings contendo os valores alvo da busca.
 * @return Retorna um ponteiro para a LISTA criada com as queries encapsuladas.
 */
LISTA* where_interno(int mn, char** chaves, char** valores);

#endif