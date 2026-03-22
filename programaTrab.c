#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HEADER 17
#define TAM_REGISTRO 80
#define CHAR_LIXO '$'
#define MAX_ESTACOES 1000

typedef struct
{
	char status;		 // '0' para inconsistente, '1' para consistente
	int topo;			 // RRN do topo da pilha de registros removidos. iniciado com -1
	int proxRRN;		 // RRN do próximo registro a ser inserido
	int nroEstacoes;	 // Número de estações cadastradas
	int nroParesEstacao; // Número de pares de estações cadastrados
} Header;

typedef struct
{
	char removido;		 // '0' para registro existente, '1' para registro removido
	int proximo;		 // RRN do próximo registro na pilha de registros removidos (apenas para registros removidos)
	int codEstacao;		 // Código da estação (Não aceita valor 0)
	int codLinha;		 // Código da linha a qual a estação pertence
	int codProxEstacao;	 // Código da próxima estação na mesma linha
	int distProxEstacao; // Distância para a próxima estação na mesma linha
	int codLinhaIntegra; // Código da linha de integração (0 se não houver integração)
	int codEstIntegra;	 // Código da estação de integração (0 se não houver integração)
	int tamNomeEstacao;	 // Tamanho do nome da estação
	char *nomeEstacao;	 // Nome da estação (string de tamanho variável) (Não aceita valor 0)
	int tamNomeLinha;	 // Tamanho do nome da linha
	char *nomeLinha;	 // Nome da linha (string de tamanho variável)
} Estacao;

typedef struct
{
	int origem;
	int destino;
} ParEstacoes;

char buffer[TAM_REGISTRO];

void mostrar_erro()
{
	printf("Falha no processamento do arquivo.\n");
}

void mostrar_buffer_como_bytes()
{
	for (int i = 0; i < TAM_REGISTRO; i++)
	{
		printf("%02X ", (unsigned char)buffer[i]);
		if ((i + 1) % 4 == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
}

int comparar_pares(const void *a, const void *b)
{
	ParEstacoes *p1 = (ParEstacoes *)a;
	ParEstacoes *p2 = (ParEstacoes *)b;

	if(p1->origem != p2->origem) {
		return p1->origem - p2->origem;
	}

	return p1->destino - p2->destino;
}

int escrever_estacao_no_buffer(Estacao *estacao)
{
	int offset = 0;

	memset(buffer, CHAR_LIXO, TAM_REGISTRO);

	memcpy(buffer + offset, &estacao->removido, sizeof(estacao->removido));
	offset += sizeof(estacao->removido);
	memcpy(buffer + offset, &estacao->proximo, sizeof(estacao->proximo));
	offset += sizeof(estacao->proximo);
	memcpy(buffer + offset, &estacao->codEstacao, sizeof(estacao->codEstacao));
	offset += sizeof(estacao->codEstacao);
	memcpy(buffer + offset, &estacao->codLinha, sizeof(estacao->codLinha));
	offset += sizeof(estacao->codLinha);
	memcpy(buffer + offset, &estacao->codProxEstacao, sizeof(estacao->codProxEstacao));
	offset += sizeof(estacao->codProxEstacao);
	memcpy(buffer + offset, &estacao->distProxEstacao, sizeof(estacao->distProxEstacao));
	offset += sizeof(estacao->distProxEstacao);
	memcpy(buffer + offset, &estacao->codLinhaIntegra, sizeof(estacao->codLinhaIntegra));
	offset += sizeof(estacao->codLinhaIntegra);
	memcpy(buffer + offset, &estacao->codEstIntegra, sizeof(estacao->codEstIntegra));
	offset += sizeof(estacao->codEstIntegra);
	memcpy(buffer + offset, &estacao->tamNomeEstacao, sizeof(estacao->tamNomeEstacao));
	offset += sizeof(estacao->tamNomeEstacao);
	memcpy(buffer + offset, estacao->nomeEstacao, estacao->tamNomeEstacao);
	offset += estacao->tamNomeEstacao;
	memcpy(buffer + offset, &estacao->tamNomeLinha, sizeof(estacao->tamNomeLinha));
	offset += sizeof(estacao->tamNomeLinha);
	memcpy(buffer + offset, estacao->nomeLinha, estacao->tamNomeLinha);
	offset += estacao->tamNomeLinha;

	return offset; // Retorna o número de bytes escritos no buffer
}

int escrever_header_no_arquivo(FILE *f, Header *header)
{
	fseek(f, 0, SEEK_SET);
	fwrite(&header->status, sizeof(header->status), 1, f);
	fwrite(&header->topo, sizeof(header->topo), 1, f);
	fwrite(&header->proxRRN, sizeof(header->proxRRN), 1, f);
	fwrite(&header->nroEstacoes, sizeof(header->nroEstacoes), 1, f);
	fwrite(&header->nroParesEstacao, sizeof(header->nroParesEstacao), 1, f);
	return 0;
}

void mostrar_bytes_do_arquivo(FILE *f, int num_bytes)
{
	fseek(f, 0, SEEK_SET);
	unsigned char byte;
	for (int i = 0; i < num_bytes; i++)
	{
		fread(&byte, sizeof(byte), 1, f);
		printf("%02X ", byte);
		if ((i + 1) % 4 == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
}

void imprimir_estacao(Estacao *estacao)
{
	printf("Código da estação: %d\n", estacao->codEstacao);
	printf("Nome da estação: %s\n", estacao->nomeEstacao);
	printf("Código da linha: %d\n", estacao->codLinha);
	printf("Nome da linha: %s\n", estacao->nomeLinha);
	printf("Código da próxima estação: %d\n", estacao->codProxEstacao);
	printf("Distância para a próxima estação: %d\n", estacao->distProxEstacao);
	printf("Código da linha de integração: %d\n", estacao->codLinhaIntegra);
	printf("Código da estação de integração: %d\n", estacao->codEstIntegra);
}

void escrever_buffer_no_arquivo(FILE *f, char *buffer)
{
	fwrite(buffer, sizeof(char), TAM_REGISTRO, f);
}

char *obter_proximo_campo(char **ponteiro_linha)
{
	char *inicio = *ponteiro_linha;

	if (inicio == NULL)
		return NULL;

	char *virgula = strchr(inicio, ',');

	if (virgula != NULL)
	{
		*virgula = '\0';
		*ponteiro_linha = virgula + 1;
	}
	else
	{
		char *quebra_linha = strpbrk(inicio, "\r\n");
		if (quebra_linha)
			*quebra_linha = '\0';

		*ponteiro_linha = NULL;
	}

	return inicio;
}

// CodEstacao,NomeEstacao,CodLinha,NomeLinha,CodProxEst,DistanciaProxEst,CodLinhaInteg,CodEstacaoInteg

int linha_csv_para_estacao(char *linha_csv, Estacao *estacao)
{
	char *ponteiro_linha = linha_csv;
	char *token;

	token = obter_proximo_campo(&ponteiro_linha);
	if (token == NULL)
		return -1;
	estacao->codEstacao = atoi(token);

	token = obter_proximo_campo(&ponteiro_linha);
	if (token == NULL)
		return -1;
	estacao->tamNomeEstacao = strlen(token);
	estacao->nomeEstacao = strdup(token);

	token = obter_proximo_campo(&ponteiro_linha);
	if (token == NULL)
		return -1;
	estacao->codLinha = atoi(token) == 0 ? -1 : atoi(token);

	token = obter_proximo_campo(&ponteiro_linha);
	if (token == NULL)
		return -1;
	estacao->tamNomeLinha = strlen(token);
	estacao->nomeLinha = strdup(token);

	token = obter_proximo_campo(&ponteiro_linha);
	if (token == NULL)
		return -1;
	estacao->codProxEstacao = atoi(token) == 0 ? -1 : atoi(token);

	token = obter_proximo_campo(&ponteiro_linha);
	if (token == NULL)
		return -1;
	estacao->distProxEstacao = atoi(token) == 0 ? -1 : atoi(token);

	token = obter_proximo_campo(&ponteiro_linha);
	if (token == NULL)
		return -1;
	estacao->codLinhaIntegra = atoi(token) == 0 ? -1 : atoi(token);

	token = obter_proximo_campo(&ponteiro_linha);
	if (token == NULL)
		return -1;
	estacao->codEstIntegra = atoi(token) == 0 ? -1 : atoi(token);

	return 0;
}

int criar_arquivo_binario(const char *nome_arquivo_csv, const char *nome_arquivo_binario)
{
	FILE *csv = fopen(nome_arquivo_csv, "r");
	if (csv == NULL)
	{
		mostrar_erro();
		return EXIT_FAILURE;
	}

	fseek(csv, 101, SEEK_SET); // Pula os primeiros 101 bytes do arquivo CSV

	FILE *bin = fopen(nome_arquivo_binario, "wb+");
	if (bin == NULL)
	{
		mostrar_erro();
		fclose(csv);
		return EXIT_FAILURE;
	}

	Header header = {'1', -1, 0, 0, 0};

	escrever_header_no_arquivo(bin, &header);

	char linha[256];
	// fgets(linha, sizeof(linha), csv);

	escrever_header_no_arquivo(bin, &header);

	int contador_estacoes = 0;
	int total_pares_lidos = 0;

	ParEstacoes pares[MAX_ESTACOES];

	while (fgets(linha, sizeof(linha), csv))
	{
		Estacao *estacao = (Estacao *)malloc(sizeof(Estacao));
		if (estacao == NULL)
		{
			mostrar_erro();
			fclose(csv);
			return EXIT_FAILURE;
		}

		int err = linha_csv_para_estacao(linha, estacao);
		if (err != 0)
		{
			mostrar_erro();
			fclose(csv);
			return EXIT_FAILURE;
		}

		escrever_estacao_no_buffer(estacao);

		escrever_buffer_no_arquivo(bin, buffer);

		if (estacao->codProxEstacao != -1)
		{
			pares[total_pares_lidos].origem = estacao->codEstacao;
			pares[total_pares_lidos].destino = estacao->codProxEstacao;
			total_pares_lidos++;
		}

		// if(contador_estacoes == 0) {
		// 	mostrar_buffer_como_bytes();
		// }

		contador_estacoes++;

		// imprimir_estacao(estacao);
	}

	/*
		pares[] registra todos os diferentes pares de estacao-proxEstacao.
		para encontrar o número de diferentes pares, ordena-se o vetor e o percorre.
		caso duas linhas consecutivas diferem de origem ou destino, contabiliza +1 pro número de pares.
	*/

	qsort(pares, total_pares_lidos, sizeof(ParEstacoes), comparar_pares);
	
	int nroParesEstacao = 0;
	if (total_pares_lidos > 0)
	{
		nroParesEstacao = 1;
		for (int i = 1; i < total_pares_lidos; i++)
		{
			if (pares[i].origem != pares[i - 1].origem || pares[i].destino != pares[i - 1].destino)
			{
				nroParesEstacao++;
			}
		}
	}

	fseek(bin, 0, SEEK_SET);
	header.nroEstacoes = contador_estacoes;
	header.nroParesEstacao = nroParesEstacao;

	printf("Número de estações: %d\n", header.nroEstacoes);
	printf("Número de pares de estação: %d\n", header.nroParesEstacao);

	escrever_header_no_arquivo(bin, &header);

	mostrar_bytes_do_arquivo(bin, 17);

	fclose(csv);
	fclose(bin);
	return EXIT_SUCCESS;
}

Header *ler_header_do_arquivo(FILE *f)
{
	Header *header = (Header *)malloc(sizeof(Header));
	fseek(f, 0, SEEK_SET);
	fread(&header->status, sizeof(header->status), 1, f);
	fread(&header->topo, sizeof(header->topo), 1, f);
	fread(&header->proxRRN, sizeof(header->proxRRN), 1, f);
	fread(&header->nroEstacoes, sizeof(header->nroEstacoes), 1, f);
	fread(&header->nroParesEstacao, sizeof(header->nroParesEstacao), 1, f);
	return header;
}

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

	// mostrar_bytes_do_arquivo(f, 100);

	// escrever_estacao_no_buffer(estacao);

	// mostrar_buffer_como_bytes();

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