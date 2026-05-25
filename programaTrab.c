#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CONSTS.h"

#include "utils/utils.h"
#include "Header/Header.h"

#include "CRUD/CRUD.h"

/*

	Arthur Santana - 16818912
	Arthur Martins Pereira - 16855601

	1. CREATE - Santana
	2. select FROM - Martins
	3. select WHERE - Martins
	4. delete - Santana
	5. INSERT INTO - Martins
	6. update - Santana

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
		err = SELECT_ALL(f);
		break;

	// SELECT WHERE
	case 3:
		scanf("%s", nome_arquivo_binario);
		f = fopen(nome_arquivo_binario, "rb");
		scanf("%d", &n);
		SELECT(n, f);
		break;

	// DELETE
	case 4:
		scanf("%s", nome_arquivo_binario);
		scanf("%d", &n);
		f = fopen(nome_arquivo_binario, "rb+");
		err = DELETE(n, f);
		break;

	// INSERT INTO
	case 5:
		scanf("%s", nome_arquivo_binario);
		f = fopen(nome_arquivo_binario, "rb+");
		err = INSERT(f);
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

		err = UPDATE(n, f);
		break;
	}

	if(f != NULL)
		fclose(f);

	if (err == 1)
	{
		mostrar_erro();
		return 0;
	}

	if (opcode != 1 && opcode != 2 && opcode != 3)
	{
		BinarioNaTela(nome_arquivo_binario);
	}

	return 0;
}