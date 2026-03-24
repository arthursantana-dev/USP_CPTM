#ifndef DEBUG_H
#define DEBUG_H

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <ctype.h>

	#include "../CONSTS.h"
	#include "../estacao/estacao.h"

	void mostrar_erro();

	void utils_mostrar_buffer_como_bytes(char *buffer);

	void utils_imprimir_estacao(Estacao *estacao);

	void utils_mostrar_bytes_do_arquivo(FILE *f, int num_bytes);

	void utils_linha_para_estacao(Estacao *estacao, char *linha);

#endif


