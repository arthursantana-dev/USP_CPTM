#ifndef BUFFER_H
	#define BUFFER_H

	#include <string.h>
	#include <stdlib.h>

	#include "../CONSTS.h"
	#include "../estacao/estacao.h"
	#include "../ParEstacoes/ParEstacoes.h"

	char *criar_buffer();

	int escrever_estacao_no_buffer(Estacao *estacao, char *buffer);

#endif