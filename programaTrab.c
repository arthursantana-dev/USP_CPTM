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

	Arthur Santana - 16818912
	Arthur Martins Pereira - 16855601

	1. CREATE - Santana
	2. SELECT FROM - Martins
	3. SELECT WHERE - Martins
	4. DELETE - Santana
	5. INSERT INTO - Martins
	6. UPDATE - Santana

	Obs.:

	- mexer nos pares e nroEstacoes somente nos DELETES.
	- garantir acesso ao vídeo explicando o projeto.
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

	int opcode;

	char nome_arquivo_binario[MAX_TAM_NOME];
	int n;

	scanf("%d", &opcode);

	FILE *f = NULL;

	int err = 0;

	switch (opcode)
	{
	// CREATE
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
		scanf("%d", &n);
		for (int i = 0; i < n; i++)
		{
			LISTA *resultados = SELECT(where(), f);
			lista_imprimir(resultados, (void (*)(void *))utils_imprimir_estacao_ln);
			printf("\n");
			lista_apagar(&resultados, (void (*)(void *))destruir_estacao);
		}
		break;

	// DELETE
	case 4:
		scanf("%s", nome_arquivo_binario);
		scanf("%d", &n);

		f = fopen(nome_arquivo_binario, "rb+");

		for (int i = 0; i < n; i++)
		{
			Estacao *estacao = criar_estacao_para_busca(0, "", 0, "", 0, 0, 0, 0);
			ler_input_para_estacao_de_busca(estacao);
			err = DELETE(estacao, f);
			if(err) break;
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
		scanf("%s", nome_arquivo_binario);
		scanf("%d", &n);

		f = fopen(nome_arquivo_binario, "rb+");

		if (f == NULL)
		{
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

			err = UPDATE(estacao_busca, estacao_valores, f);

			if(err) break;

			destruir_estacao(estacao_busca);
			destruir_estacao(estacao_valores);

		}
		break;
	}

	if(err == 1){
		mostrar_erro();
		return 0;
	}

	if (opcode != 1 && opcode != 2 && opcode != 3)
	{
		fclose(f);
		BinarioNaTela(nome_arquivo_binario);
	}

	return 0;
}