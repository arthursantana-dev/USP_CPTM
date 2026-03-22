#ifndef ESTACAO_H
#define ESTACAO_H

	typedef struct
	{
		char removido;		 // '0' para registro existente, '1' para registro removido
		int proximo;		 // RRN do próximo registro na pilha de registros removidos (apenas para registros removidos)
		int codEstacao;		 // Código da estação (Não aceita valor 0)
		int codLinha;		 // Código da linha a qual a estação pertence
		int codProxEstacao;	 // Código da próxima estação na mesma linha
		int distProxEstacao; // Distância para a próxima estação na mesma linha
		int codLinhaIntegra; // Código da linha de integração (0 se não houver integração)
		int codEstIntegra;	 // Código da estação de integração (0 se não houver integração)
		int tamNomeEstacao;	 // Tamanho do nome da estação
		char *nomeEstacao;	 // Nome da estação (string de tamanho variável) (Não aceita valor 0)
		int tamNomeLinha;	 // Tamanho do nome da linha
		char *nomeLinha;	 // Nome da linha (string de tamanho variável)
	} Estacao;

#endif