#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CONSTS.h"

#include "Header/Header.h"

#include "IO/IO.h"

#include "CRUD/CRUD.h"

/*

	Arthur Santana - 16818912
	Arthur Martins Pereira - 16855601

	1. CREATE - Santana
	2. SELECT FROM - Martins
	3. SELECT WHERE - Martins
	4. DELETE - Santana
	5. INSERT INTO - Martins
	6. UPDATE - Santana

	Usando índice (arvore b):

	7. CREATE INDEX - Santana
	8. SELECT WHERE - Martins
	9. INSERT INTO - Martins
	10.DELETE - Santana

	Obs.:

	- mexer nos pares e nroEstacoes somente nos deleteS.
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
	char nome_arquivo_btree[MAX_TAM_NOME];

	int n;

	scanf("%d", &opcode);

	FILE *f_dados = NULL;
	FILE *f_ab = NULL;

	int err = 0;

	switch (opcode)
	{
	// CREATE
	case 1:
		char nome_arquivo_csv[MAX_TAM_NOME];
		scanf("%s", nome_arquivo_csv);
		scanf("%s", nome_arquivo_binario);
		err = CREATE(nome_arquivo_csv, nome_arquivo_binario);
		break;

	// SELECT ALL
	case 2:
		scanf("%s", nome_arquivo_binario);
		f_dados = fopen(nome_arquivo_binario, "rb");
		err = SELECT_ALL(f_dados);
		break;

	// SELECT ALL
	case 3:
		scanf("%s", nome_arquivo_binario);

		f_dados = fopen(nome_arquivo_binario, "rb");

		scanf("%d", &n);
		err = SELECT_SEM_INDICE(n, f_dados);

		break;

	// DELETE
	case 4:
		scanf("%s", nome_arquivo_binario);
		scanf("%d", &n);

		f_dados = fopen(nome_arquivo_binario, "rb+");
		err = DELETE_SEM_INDICE(n, f_dados);
		break;

	// INSERT INTO
	case 5:
		scanf("%s", nome_arquivo_binario);

		f_dados = fopen(nome_arquivo_binario, "rb+");

		err = INSERT_SEM_INDICE(f_dados);
		if (!err)
			atualizar_nros_estacoes_no_header(f_dados);
		break;

	// UPDATE
	case 6:
		scanf("%s", nome_arquivo_binario);
		scanf("%d", &n);

		f_dados = fopen(nome_arquivo_binario, "rb+");

		err = UPDATE(n, f_dados);
		break;

	// CREATE INDEX
	case 7:
		scanf("%s", nome_arquivo_binario);
		scanf("%s", nome_arquivo_btree);

		// printf("%s %s\n", nome_arquivo_binario, nome_arquivo_btree);

		err = CREATE_INDEX(nome_arquivo_binario, nome_arquivo_btree);
		break;

	// SELECT WHERE
	case 8:
		scanf("%s", nome_arquivo_binario);
		scanf("%s", nome_arquivo_btree);

		f_dados = fopen(nome_arquivo_binario, "rb");
		f_ab = fopen(nome_arquivo_btree, "rb");

		scanf("%d", &n);
		err = SELECT(n, f_dados, f_ab);
		break;

	// INSERT INTO
	case 9:
		scanf("%s", nome_arquivo_binario);
		scanf("%s", nome_arquivo_btree);

		f_dados = fopen(nome_arquivo_binario, "rb+");
		f_ab = fopen(nome_arquivo_btree, "rb+");

		err = INSERT(f_dados, f_ab);
		if (!err)
			atualizar_nros_estacoes_no_header(f_dados);
		break;

	// DELETE usando a estrutura de índices
	case 10:
		scanf("%s", nome_arquivo_binario);
		scanf("%s", nome_arquivo_btree);

		scanf("%d", &n);
		f_dados = fopen(nome_arquivo_binario, "rb+");
		f_ab = fopen(nome_arquivo_btree, "rb+");

		if (f_ab == NULL)
		{
			err = EXIT_FAILURE;
			mostrar_erro();
			break;
		}

		err = DELETE(n, f_dados, f_ab);

		if (!err)
			atualizar_nros_estacoes_no_header(f_dados);
		break;
	}

	if (f_dados != NULL)
		fclose(f_dados);

	if (f_ab != NULL)
		fclose(f_ab);

	if (err == 1)
	{
		mostrar_erro();
		return 0;
	}

	if (opcode != 1 && opcode != 2 && opcode != 3 && opcode != 7 && opcode != 8)
	{
		BinarioNaTela(nome_arquivo_binario);
	}

	if (opcode >= 9 && opcode <= 10)
	{
		BinarioNaTela(nome_arquivo_btree);
	}

	return 0;
}