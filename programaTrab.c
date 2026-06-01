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
	8. SELECT WHERE
	9. DELETE  - Santana
	10.INSERT INTO

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
	char nome_arquivo_arvore_b[MAX_TAM_NOME];

	int n;

	scanf("%d", &opcode);

	FILE *f = NULL;
	FILE *fab = NULL;

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
		err = SELECT(n, f);
		break;

	// DELETE
	case 4:
		scanf("%s", nome_arquivo_binario);
		scanf("%d", &n);
		f = fopen(nome_arquivo_binario, "rb+");
		err = DELETE(n, f, NULL);
		break;

	// DELETE
	case 5:
		scanf("%s", nome_arquivo_binario);
		f = fopen(nome_arquivo_binario, "rb+");
		err = INSERT(f);
		break;
		
	// INSERT INTO
	case 6:
		scanf("%s", nome_arquivo_binario);
		scanf("%d", &n);

		f = fopen(nome_arquivo_binario, "rb+");
		err = UPDATE(n, f);
		break;

	case 7:
		scanf("%s", nome_arquivo_binario);
		scanf("%s", nome_arquivo_arvore_b);

		// printf("%s %s\n", nome_arquivo_binario, nome_arquivo_arvore_b);

		err = CREATE_INDEX(nome_arquivo_binario, nome_arquivo_arvore_b);
		break;

	case 9: // A deleção usando a estrutura de índices e cuidada internamente (crud_delete)
		scanf("%s", nome_arquivo_binario);
		scanf("%s", nome_arquivo_arvore_b);

		scanf("%d", &n);
		f = fopen(nome_arquivo_binario, "rb+");
		fab = fopen(nome_arquivo_arvore_b, "rb+");

		if(fab == NULL){
			err = EXIT_FAILURE;
			mostrar_erro();
			break;
		}
		
		err = DELETE(n, f, fab);
		break;

	}

	if(f != NULL)
		fclose(f);

	if(fab != NULL)
		fclose(fab);

	if (err == 1)
	{
		mostrar_erro();
		return 0;
	}

	if (opcode != 1 && opcode != 2 && opcode != 3 && opcode != 7)
	{
		BinarioNaTela(nome_arquivo_binario);
		BinarioNaTela(nome_arquivo_arvore_b);
	}

	return 0;
}