#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CONSTS.h"

#include "CREATE/CREATE.h"
#include "DELETE/DELETE.h"
#include "UPDATE/UPDATE.h"
#include "SELECT/SELECT.h"
#include "INSERT/INSERT.h"

#include "utils/utils.h"
#include "buffer/buffer.h"
#include "Header/Header.h"
#include "SetNomes/SetNomes.h"

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

	// FILE *f = fopen("estacoes.bin", "rb+");

	// if (f == NULL)
	// {
	// 	perror("Erro ao abrir o arquivo");
	// 	return EXIT_FAILURE;
	// }

	int opcode;

	char nome_arquivo_binario[MAX_TAM_NOME];
	int n;

	scanf("%d", &opcode);

	FILE *f = NULL;

	// BinarioNaTela("gabarito1.bin");

	switch (opcode)
	{
	// CREATE
	// exemplo:
	// 1 e.csv estacoes.bin
	case 1:
		char nome_arquivo_csv[MAX_TAM_NOME];
		scanf("%s", nome_arquivo_csv);
		scanf("%s", nome_arquivo_binario);
		CREATE(nome_arquivo_csv, nome_arquivo_binario);
		break;
	// SELECT FROM
	case 2:
		scanf("%s", nome_arquivo_binario);
		f = fopen(nome_arquivo_binario, "rb");
		select_all(f);
		break;
	// SELECT WHERE
	case 3:
		scanf("%s", nome_arquivo_binario);
		f = fopen(nome_arquivo_binario, "rb");
		int n;
		scanf("%d", &n);
		for(int i=0;i<n;i++){
			LISTA* resultados = SELECT(where(), f);
			lista_imprimir(resultados, (void * (*)(void *)) utils_imprimir_estacao_ln);
			lista_apagar(&resultados, (void (*)(void *)) destruir_estacao);
		}
		break;

		// DELETE
		// exemplo:
		/*
	4 estacoes.bin 2
	1 codEstacao 11
	1 nomeEstacao "Luz"
		*/

	case 4:
		// char nome_arquivo_binario[MAX_TAM_NOME];
		scanf("%s", nome_arquivo_binario);
		scanf("%d", &n);
		// printf("Número de estações a buscar: %d\n", n);

		f = fopen(nome_arquivo_binario, "rb+");

		for (int i = 0; i < n; i++)
		{
			Estacao *estacao = criar_estacao_para_busca(0, "", 0, "", 0, 0, 0, 0);
			ler_input_para_estacao_de_busca(estacao);
			DELETE(estacao, f);
			// utils_imprimir_estacao_ln(estacao);
			// printf("--------------\n");
			destruir_estacao(estacao);
		}

		break;
	// INSERT INTO
	case 5:
		scanf("%s", nome_arquivo_binario);
		f = fopen(nome_arquivo_binario, "rb+");
		INSERT(f);
		break;
	// UPDATE
	case 6:

		// char nome_arquivo_binario[MAX_TAM_NOME];
		scanf("%s", nome_arquivo_binario);
		scanf("%d", &n);
		// printf("Número de estações a buscar: %d\n", n);

		f = fopen(nome_arquivo_binario, "rb+");

		if (f == NULL)
		{
			// printf("%d\n", 1);
			mostrar_erro();
			return EXIT_FAILURE;
		}

		for (int i = 0; i < n; i++)
		{
			// valores a serem buscados
			Estacao *estacao_busca = criar_estacao_para_busca(0, "", 0, "", 0, 0, 0, 0);

			// valores a serem substituidos
			Estacao *estacao_valores = criar_estacao_para_busca(0, "", 0, "", 0, 0, 0, 0);

			ler_input_para_estacao_de_busca(estacao_busca);
			ler_input_para_estacao_de_busca(estacao_valores);

			UPDATE(nome_arquivo_binario, estacao_busca, estacao_valores, f);
			destruir_estacao(estacao_busca);
			destruir_estacao(estacao_valores);

			// utils_imprimir_estacao_ln(estacao_busca);
			// printf("--------------\n");
		}

		// printf("Conseguido:");
		// BinarioNaTela(nome_arquivo_binario);

		// printf("\n\nEsperado:");
		// printf("11503.670000");

		break;
	}

	if(opcode != 1){
		fclose(f);
		BinarioNaTela(nome_arquivo_binario);
	}

	return 0;
}