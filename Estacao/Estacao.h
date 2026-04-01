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

	Estacao *criar_estacao(int codEstacao, char *nomeEstacao, int codLinha, char *nomeLinha, int codProxEstacao, int distProxEstacao, int codLinhaIntegra, int codEstacaoIntegra);

	Estacao *criar_estacao_para_busca(int codEstacao, char *nomeEstacao, int codLinha, char *nomeLinha, int codProxEstacao, int distProxEstacao, int codLinhaIntegra, int codEstacaoIntegra);

	int comparar_estacoes(Estacao *ea, Estacao *eb);

	void editar_estacao(Estacao *estacao, Estacao *novos_valores);

	void destruir_estacao(Estacao *estacao);

#endif