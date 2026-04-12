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

/**
 * @brief Recupera e exibe de forma sequencial os dados de todos os registros existentes no arquivo binário.
 * @param f Ponteiro para o arquivo binário aberto para leitura.
 * @return retorna um inteiro indicando o sucesso (0) ou falha (1) da operação.
 */
int select_all(FILE *f);

/**
 * @brief Realiza a busca sequencial de registros no arquivo binário utilizando uma estrutura como "molde" de critérios e imprime os resultados imediatamente na tela ("on the fly"), sem alocação em listas.
 * @param estacao_selecao Ponteiro para a estrutura Estacao preenchida apenas com os valores que devem ser utilizados como critério de busca (filtros).
 * @param f Ponteiro para o arquivo binário de dados já aberto em modo de leitura.
 * @return Retorna EXIT_SUCCESS em caso de sucesso da operação de leitura, ou EXIT_FAILURE caso o arquivo ou cabeçalho sejam inválidos ou nulos.
 */
int SELECT(Estacao *estacao_selecao, FILE *f);


#endif