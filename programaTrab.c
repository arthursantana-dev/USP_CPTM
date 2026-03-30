#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CONSTS.h"

#include "CREATE/CREATE.h"
#include "DELETE/DELETE.h"
#include "UPDATE/UPDATE.h"

#include "utils/utils.h"
#include "buffer/buffer.h"
#include "Header/Header.h"
#include "SetNomes/SetNomes.h"

#include "fornecidas.c"

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

	int codEstacoesRemocao[] = {78, 29, 42, 139, 98, 116, 76, 87, 33, 112, 188, 65, 12, 7, 115, 5, 126, 120, 61, 167, 58, 140, 129, 35, 89, 156, 109, 174, 38, 185, 200, 39, 32, 88, 186, 191, 24, 155, 34, 165, 9, 4, 143, 79, 132, 125, 11, 149, 19, 66, 104, 23, 52};

	criar_arquivo_binario("e.csv", "estacoes.bin");

	// printf("Nomes de estações lidos do CSV:\n");
	// printf("Total de estações diferentes: %d\n", set_estacoes->tamanho);
	// for(int i = 0; i < set_estacoes->tamanho; i++) {
	// 	printf("%s\n", set_estacoes->nomes[i]);
	// }

	// for(int i = 0; i < *info_pares_estacoes->nroPares; i++) {
	// 	printf("Par %d: Origem: %d, Destino: %d\n", i+1, info_pares_estacoes->pares[i].origem, info_pares_estacoes->pares[i].destino);
	// }

	// printf("Total de pares lidos: %d\n", *tam_pares);

	// int nroRemovidos = 0;

	Estacao *estacao_busca = criar_estacao_para_busca(0, "Luz", 0, "", 0, 0, 0, 0);
	deletar_registro("estacoes.bin", estacao_busca, f);

	Estacao *ea = criar_estacao_para_busca(0, "Jardim Romano", 0, "", 0, 0, 0, 0);

	Estacao *eb = criar_estacao_para_busca(0, "Jardim Grego", 4, "Preta", 50, 2080, -1, -1);

	atualizar_registro(ea, eb, f);

	free(estacao_busca->nomeEstacao);
	free(estacao_busca->nomeLinha);
	free(estacao_busca);

	liberar_estacao(ea);
	free(ea);

	liberar_estacao(eb);
	free(eb);

	// for(int i = 0; i < sizeof(codEstacoesRemocao)/sizeof(int); i++) {

	// 	nroRemovidos++;

	// 	Estacao *estacao_busca = criar_estacao_para_busca(codEstacoesRemocao[i], "", 0, "", 0, 0, 0, 0);
	// 	deletar_registro("estacoes.bin", estacao_busca, f, info_pares_estacoes, set_estacoes);
	// 	free(estacao_busca);
	// }

	// printf("\n\n%d estações removidas.\n", nroRemovidos);

	// for(int i = 0; i < *tam_pares; i++) {
	// 	printf("Par %d: Origem: %d, Destino: %d\n", i+1, pares[i].origem, pares[i].destino);
	// }

	// deletar_registro("estacoes.bin", ea, f, info_pares_estacoes, set_estacoes);

	// BinarioNaTela("estacoes.bin");

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