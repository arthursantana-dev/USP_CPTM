#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CONSTS.h"

#include "CREATE/CREATE.h"

#include "utils/utils.h"
#include "buffer/buffer.h"
#include "Header/Header.h"

/*

	1. CREATE - Santana

	2. SELECT FROM - Martins

	3. SELECT WHERE - Martins

	4. DELETE - Santana

	5. INSERT INTO - Martins

	6. UPDATE - Santana

*/

int main()
{
	FILE *f = fopen("estacoes.bin", "rb+");

	if (f == NULL)
	{
		perror("Erro ao abrir o arquivo");
		return EXIT_FAILURE;
	}

	criar_arquivo_binario("estacoes.csv", "estacoes.bin");

	// printf("Teste de leitura do header:\n");

	// Header *header = (Header*) malloc(sizeof(Header));
	// header->status = '1';
	// header->topo = -1;
	// header->proxRRN = 0;
	// header->nroEstacoes = 0;
	// header->nroParesEstacao = 0;

	// Estacao *estacao = (Estacao*) malloc(sizeof(Estacao));
	// estacao->removido = '0';
	// estacao->proximo = -1;
	// estacao->codEstacao = 123;
	// estacao->codLinha = 456;
	// estacao->codProxEstacao = 789;
	// estacao->distProxEstacao = 10;
	// estacao->codLinhaIntegra = 0;
	// estacao->codEstIntegra = 0;
	// estacao->tamNomeEstacao = strlen("Estacao A");
	// estacao->nomeEstacao = "Estacao A";
	// estacao->tamNomeLinha = strlen("Linha 1");
	// estacao->nomeLinha = "Linha 1";

	// int offset = 0;

	// printf("Tamanho nome estação: %d\n", estacao->tamNomeEstacao);
	// printf("Tamanho nome linha: %d\n", estacao->tamNomeLinha);

	// escrever_header_no_arquivo(f, header);

	// utils_mostrar_bytes_do_arquivo(f, 100);

	// escrever_estacao_no_buffer(estacao);

	// utils_mostrar_buffer_como_bytes(buffer);

	// --------------------------------------------------------------------
	// |                    TESTE DE LEITURA DO HEADER                    |
	// --------------------------------------------------------------------

	// Header *header_lido = ler_header_do_arquivo(f);
	// printf("Status: %c\n", header_lido->status);
	// printf("Topo: %d\n", header_lido->topo);
	// printf("Próximo RRN: %d\n", header_lido->proxRRN);
	// printf("Número de estações: %d\n", header_lido->nroEstacoes);
	// printf("Número de pares de estação: %d\n", header_lido->nroParesEstacao);

	return 0;
}