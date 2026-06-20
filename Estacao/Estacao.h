#ifndef ESTACAO_H
#define ESTACAO_H

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#include "../CONSTS.h"
	#include "../Header/Header.h"

	typedef struct
	{
		char removido;		 // '0' para registro existente, '1' para registro removido
		int proximo;		 // RRN do próximo registro na pilha de registros removidos (apenas para registros removidos)
		int codEstacao;		 // código da estação (Não aceita valor nulo)
		int codLinha;		 // código da linha a qual a estação pertence
		int codProxEstacao;	 // código da próxima estação na mesma linha
		int distProxEstacao; // distância para a próxima estação na mesma linha
		int codLinhaIntegra; // código da linha de integração (-1 se não houver integração)
		int codEstacaoIntegra;	 // código da estação de integração (-1 se não houver integração)
		int tamNomeEstacao;	 // Tamanho do nome da estação
		char *nomeEstacao;	 // nome da estação (string de tamanho variável) (Não aceita valor nulo)
		int tamNomeLinha;	 // Tamanho do nome da linha
		char *nomeLinha;	 // nome da linha (string de tamanho variável)
	} Estacao;

	/**
	 * @brief Instancia e inicializa uma estrutura Estacao configurada estritamente para servir como objeto de busca, onde 0 significa ignorar o campo e -1 significa buscar por valor nulo.
	 * @param codEstacao Código da estação para busca.
	 * @param nomeEstacao Nome da estação para busca.
	 * @param codLinha Código da linha para busca.
	 * @param nomeLinha Nome da linha para busca.
	 * @param codProxEstacao Código da próxima estação para busca.
	 * @param distProxEstacao Distância para a próxima estação para busca.
	 * @param codLinhaIntegra Código da linha de integração para busca.
	 * @param codEstacaoIntegra Código da estação de integração para busca.
	 * @return Retorna um ponteiro seguro e limpo (calloc) para a Estacao de busca.
	 */
	Estacao *criar_estacao_para_busca(int codEstacao, char *nomeEstacao, int codLinha, char *nomeLinha, int codProxEstacao, int distProxEstacao, int codLinhaIntegra, int codEstacaoIntegra);

	/**
	 * @brief Compara os dados de uma estação lida em disco contra os critérios de uma estação de busca.
	 * @param ea Ponteiro para a estação configurada com os critérios de busca (valores alvo).
	 * @param eb Ponteiro para a estação "candidata", extraída do arquivo de dados.
	 * @return Retorna 1 se a estação candidata satisfizer as condições de busca, ou 0 caso contrário.
	 */
	int comparar_estacoes(Estacao *ea, Estacao *eb);

	void ordenar_estacoes(Estacao* estacoes[], int numero_estacoes, char *campo_ordenacao);

	/**
	 * @brief Atualiza in-place os dados de uma estação existente, substituindo apenas os campos explicitamente informados em novos_valores.
	 * @param estacao Ponteiro para a estação original que terá seus dados editados.
	 * @param novos_valores Ponteiro para a estação contendo os campos de substituição (campos com 0 são ignorados).
	 */
	void copiar_estacao(Estacao *estacao, Estacao *novos_valores);

	void set_estacao(Estacao *estacao, int codEstacao, char *nomeEstacao, int codLinha, char *nomeLinha, int codProxEstacao, int distProxEstacao, int codLinhaIntegra, int codEstacaoIntegra);

	void imprimir_join_estacoes(Estacao* estacao_A, Estacao* estacao_B);

	/**
	 * @brief Libera de forma segura toda a memória dinâmica (strings alocadas e struct) atrelada à estação.
	 * @param estacao Ponteiro para a estrutura Estacao a ser totalmente liberada e destruída.
	 */
	void destruir_estacao(Estacao *estacao);

	void limpar_estacao(Estacao *estacao);

	void set_valores_estacao_para_busca(Estacao *estacao);

	/*Buffer de Estação*/

	char *criar_buffer();

	int escrever_buffer_na_estacao(char *buffer, Estacao *estacao);

	int escrever_estacao_no_buffer(Estacao *estacao, char *buffer);

	void escrever_buffer_no_arquivo(FILE *f_dados, char *buffer);

	void atualizar_nros_estacoes_no_header(FILE *f_dados);

#endif