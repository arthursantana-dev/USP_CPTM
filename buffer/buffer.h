#ifndef BUFFER_H
	#define BUFFER_H

	#include <string.h>
	#include <stdlib.h>

	#include "../CONSTS.h"
	#include "../Estacao/Estacao.h"
	#include "../utils/utils.h"
	#include "../ParEstacoes/ParEstacoes.h"

	/**
	 * @brief Aloca dinamicamente o espaço base na memória para o tamanho exato de um registro fixo do projeto (80 bytes). 
	 * @return Retorna um array de caracteres alocado pronto para uso em leituras ou escritas.
	 */
	char *criar_buffer();

	/**
	 * @brief Serializa os dados literais da estrutura Estacao em uma cadeia de bytes fixos, substituindo o espaço sobressalente por lixo de memória ('$').
	 * @param estacao Ponteiro para a estrutura da qual os dados serão extraídos.
	 * @param buffer Ponteiro para o array pré-alocado (buffer) onde os bytes serão assinalados na devida ordem e padding.
	 * @return Retorna a quantidade de bytes total percorrida e inserida (offset final).
	 */
	int escrever_estacao_no_buffer(Estacao *estacao, char *buffer);

	/**
	 * @brief Processo reverso: decodifica um array de bytes contínuo do arquivo transcrevendo as seções binárias de volta para a estrutura Estacao em memória.
	 * @param buffer Ponteiro para a cadeia de bytes puramente lida pelo "fread" de um registro de 80 bytes.
	 * @param estacao Estrutura recipiente que vai alocar e segmentar os dados desserializados.
	 * @return Retorna 1 se o parser do buffer funcionou sem intercorrências, ou 0 em caso de erro.
	 */
	int escrever_buffer_na_estacao(char* buffer, Estacao *estacao);

#endif