#ifndef ESTACAO_H
#define ESTACAO_H

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	typedef struct
	{
		char removido;		 // '0' para registro existente, '1' para registro removido
		int proximo;		 // RRN do próximo registro na pilha de registros removidos (apenas para registros removidos)
		int codEstacao;		 // Código da estação (Não aceita valor nulo)
		int codLinha;		 // Código da linha a qual a estação pertence
		int codProxEstacao;	 // Código da próxima estação na mesma linha
		int distProxEstacao; // Distância para a próxima estação na mesma linha
		int codLinhaIntegra; // Código da linha de integração (-1 se não houver integração)
		int codEstacaoIntegra;	 // Código da estação de integração (-1 se não houver integração)
		int tamNomeEstacao;	 // Tamanho do nome da estação
		char *nomeEstacao;	 // Nome da estação (string de tamanho variável) (Não aceita valor nulo)
		int tamNomeLinha;	 // Tamanho do nome da linha
		char *nomeLinha;	 // Nome da linha (string de tamanho variável)
	} Estacao;

	/**
	 * @brief Instancia e inicializa uma estrutura Estacao padrão; campos int com valor 0 são convertidos em nulos (-1).
	 * @param codEstacao Código da estação.
	 * @param nomeEstacao Nome da estação (string).
	 * @param codLinha Código da linha.
	 * @param nomeLinha Nome da linha (string).
	 * @param codProxEstacao Código da próxima estação.
	 * @param distProxEstacao Distância para a próxima estação.
	 * @param codLinhaIntegra Código da linha de integração.
	 * @param codEstacaoIntegra Código da estação de integração.
	 * @return Retorna um ponteiro para a nova Estacao alocada na memória (malloc).
	 */
	Estacao *criar_estacao(int codEstacao, char *nomeEstacao, int codLinha, char *nomeLinha, int codProxEstacao, int distProxEstacao, int codLinhaIntegra, int codEstacaoIntegra);

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

	/**
	 * @brief Atualiza in-place os dados de uma estação existente, substituindo apenas os campos explicitamente informados em novos_valores.
	 * @param estacao Ponteiro para a estação original que terá seus dados editados.
	 * @param novos_valores Ponteiro para a estação contendo os campos de substituição (campos com 0 são ignorados).
	 */
	void editar_estacao(Estacao *estacao, Estacao *novos_valores);

	/**
	 * @brief Libera de forma segura toda a memória dinâmica (strings alocadas e struct) atrelada à estação.
	 * @param estacao Ponteiro para a estrutura Estacao a ser totalmente liberada e destruída.
	 */
	void destruir_estacao(Estacao *estacao);
#endif