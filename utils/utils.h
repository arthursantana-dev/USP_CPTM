#ifndef DEBUG_H
#define DEBUG_H

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <ctype.h>

	#include "../CONSTS.h"
	#include "../Estacao/Estacao.h"
	#include "../Header/Header.h"
	#include "../buffer/buffer.h"

	void mostrar_erro();

	void utils_mostrar_buffer_como_bytes(char *buffer);

	void utils_imprimir_estacao(Estacao *estacao);

	void utils_imprimir_estacao_ln(Estacao *estacao);

	void utils_mostrar_bytes_do_arquivo(FILE *f, int num_bytes);

	void utils_linha_para_estacao(Estacao *estacao, char *linha);

	int utils_mostrar_pilha_remocao(FILE *f, Header *header);

	void BinarioNaTela(char *arquivo);

	void ler_input_para_estacao_de_busca(Estacao *estacao);
	
	void ScanQuoteString(char *str);

	void nullOrString(char *str);

	void nullOrInt(int *n);

	void utils_vetor_para_estacao(Estacao *estacao, char *elementos[], int qtd_elementos);

#endif


