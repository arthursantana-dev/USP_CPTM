#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CONSTS.h"

#include "CREATE/CREATE.h"
#include "DELETE/DELETE.h"
#include "SELECT/SELECT.h"

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

	Obs.:

	- evitar repetir fopen/fclose.
	- evitar usar ftell(), principalmente em loops. regs de tamanho fixo -> rrn via operações.
	- settar status como inconsistente no início de operação e consistenteao fim.
	- ignorar registros removidos (não ler de nenhum modo, somente o removido).
	- caso: remover todas as estações com o mesmo nome (contabilizar nroEstacoes e paresEstacoes).
	- usar fgets para o csv - opt.
	- gnu strsep - opt.

*/

int main()
{

	// TODO: status do arquivo (header)

	FILE *f = fopen("estacoes.bin", "rb+");

	if (f == NULL)
	{
		perror("Erro ao abrir o arquivo");
		return EXIT_FAILURE;
	}

	LISTA *resultados = select(where(), "estacoes.bin");

	lista_imprimir(resultados, (void * (*)(void *)) utils_imprimir_estacao_ln);

	// criar_arquivo_binario("estacoes.csv", "estacoes.bin");

	// Estacao *ea = criar_estacao_para_busca(0, "Tatuape", 0, "", 0, 0, 0, 0);

	// deletar_registro("estacoes.bin", ea);

	// Header *header = ler_header_do_arquivo(f);

	// utils_mostrar_pilha_remocao(f, header);

	// printf("\n\n%d\n", header->nroEstacoes);

	// Simulando o input do usuário (precisa ser um array modificável)

	// char input[200];

	// scanf("%[^\n]", input); // Lê a linha inteira, incluindo espaços

	// char input[] = "2 nomeLinha \"Verde\" codProxEstacao 27";
	// char *elementos[MAX_TOKENS];

	// int qtd_elementos = utils_decompor_linha(input, elementos);

	// // Exibindo o resultado
	// // printf("Foram encontrados %d elementos:\n", qtd_elementos);
	// for (int i = 0; i < qtd_elementos; i++) {
	//     printf("Vetor[%d]: %s\n", i, elementos[i]);
	// }

	// Estacao *estacao = criar_estacao(0, "", 0, "", 0, 0, 0, 0);

	// utils_linha_para_estacao(estacao, input);

	// utils_imprimir_estacao(estacao);

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