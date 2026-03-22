#ifndef BUFFER_H
	#define BUFFER_H

	#include <string.h>

	#include "../CONSTS.h"
	#include "../estacao/estacao.h"
	#include "../ParEstacoes/ParEstacoes.h"

	char buffer[TAM_REGISTRO];

	

	int escrever_estacao_no_buffer(Estacao *estacao)
	{
		int offset = 0;

		memset(buffer, CHAR_LIXO, TAM_REGISTRO);

		memcpy(buffer + offset, &estacao->removido, sizeof(estacao->removido));
		offset += sizeof(estacao->removido);
		memcpy(buffer + offset, &estacao->proximo, sizeof(estacao->proximo));
		offset += sizeof(estacao->proximo);
		memcpy(buffer + offset, &estacao->codEstacao, sizeof(estacao->codEstacao));
		offset += sizeof(estacao->codEstacao);
		memcpy(buffer + offset, &estacao->codLinha, sizeof(estacao->codLinha));
		offset += sizeof(estacao->codLinha);
		memcpy(buffer + offset, &estacao->codProxEstacao, sizeof(estacao->codProxEstacao));
		offset += sizeof(estacao->codProxEstacao);
		memcpy(buffer + offset, &estacao->distProxEstacao, sizeof(estacao->distProxEstacao));
		offset += sizeof(estacao->distProxEstacao);
		memcpy(buffer + offset, &estacao->codLinhaIntegra, sizeof(estacao->codLinhaIntegra));
		offset += sizeof(estacao->codLinhaIntegra);
		memcpy(buffer + offset, &estacao->codEstIntegra, sizeof(estacao->codEstIntegra));
		offset += sizeof(estacao->codEstIntegra);
		memcpy(buffer + offset, &estacao->tamNomeEstacao, sizeof(estacao->tamNomeEstacao));
		offset += sizeof(estacao->tamNomeEstacao);
		memcpy(buffer + offset, estacao->nomeEstacao, estacao->tamNomeEstacao);
		offset += estacao->tamNomeEstacao;
		memcpy(buffer + offset, &estacao->tamNomeLinha, sizeof(estacao->tamNomeLinha));
		offset += sizeof(estacao->tamNomeLinha);
		memcpy(buffer + offset, estacao->nomeLinha, estacao->tamNomeLinha);
		offset += estacao->tamNomeLinha;

		return offset; // Retorna o número de bytes escritos no buffer
	}

#endif