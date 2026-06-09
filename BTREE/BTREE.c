#include "BTREE.h"
#include <stdlib.h>

/*Cabeçalho*/

header_arvore_b arvore_b_ler_cabecalho(FILE *arquivo)
{
    header_arvore_b cabecalho;

    fseek(arquivo, 0, SEEK_SET);
    fread(&cabecalho.status, sizeof(char), 1, arquivo);
    fread(&cabecalho.no_raiz, sizeof(int), 1, arquivo);
    fread(&cabecalho.topo, sizeof(int), 1, arquivo);
    fread(&cabecalho.prox_RRN, sizeof(int), 1, arquivo);
    fread(&cabecalho.nro_nos, sizeof(int), 1, arquivo);

    return cabecalho;
}

no_arvore_b arvore_b_criar_no_vazio(void)
{
    no_arvore_b no;
    no.removido = '0';
    no.proximo = -1;
    no.tipo_no = -1;
    no.numero_chaves = 0;

    for (int i = 0; i < MAX_CHAVES; i++)
    {
        no.chaves[i] = -1;
        no.dados_byte_offsets[i] = -1;
    }
    for (int i = 0; i < ORDEM; i++)
    {
        no.filhos[i] = -1;
    }
    return no;
}

FILE *arvore_b_abrir_escrita(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "wb+");
    if (arquivo == NULL)
    {
        perror("Erro ao criar o arquivo de índice");
        exit(EXIT_FAILURE);
    }

    // Inicializa o cabeçalho com valores padrão
    header_arvore_b cabecalho = {
        .status = '0', // Inconsistente até que seja fechado corretamente
        .no_raiz = -1,
        .topo = -1,
        .prox_RRN = 0,
        .nro_nos = 0};

    // Escreve o cabeçalho inicial no arquivo
    fwrite(&cabecalho.status, sizeof(char), 1, arquivo);
    fwrite(&cabecalho.no_raiz, sizeof(int), 1, arquivo);
    fwrite(&cabecalho.topo, sizeof(int), 1, arquivo);
    fwrite(&cabecalho.prox_RRN, sizeof(int), 1, arquivo);
    fwrite(&cabecalho.nro_nos, sizeof(int), 1, arquivo);

    return arquivo;
}

FILE *arvore_b_abrir_leitura(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "rb+");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo de índice");
        exit(EXIT_FAILURE);
    }
    return arquivo;
}

no_arvore_b arvore_b_ler_no(FILE *arquivo, int rrn)
{
    no_arvore_b no;

    // Posiciona o ponteiro: Tamanho do cabecalho + (RRN * Tamanho do No)
    fseek(arquivo, TAM_CABECALHO + (rrn * TAM_NO), SEEK_SET);

    fread(&no.removido, sizeof(char), 1, arquivo);
    fread(&no.proximo, sizeof(int), 1, arquivo);
    fread(&no.tipo_no, sizeof(int), 1, arquivo);
    fread(&no.numero_chaves, sizeof(int), 1, arquivo);

    // Chaves e Referencias (Intercalados C1, PR1, C2, PR2...)
    for (int i = 0; i < MAX_CHAVES; i++)
    {
        fread(&no.chaves[i], sizeof(int), 1, arquivo);
        fread(&no.dados_byte_offsets[i], sizeof(int), 1, arquivo);
    }

    // Ponteiros das Subarvores (P1, P2, P3, P4)
    for (int i = 0; i < ORDEM; i++)
    {
        fread(&no.filhos[i], sizeof(int), 1, arquivo);
    }

    return no;
}

void arvore_b_atualizar_cabecalho(FILE *arquivo, header_arvore_b *cabecalho)
{
    if (arquivo == NULL || cabecalho == NULL)
        return;

    fseek(arquivo, 0, SEEK_SET);
    fwrite(&cabecalho->status, sizeof(char), 1, arquivo);
    fwrite(&cabecalho->no_raiz, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->topo, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->prox_RRN, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->nro_nos, sizeof(int), 1, arquivo);
}

void arvore_b_escrever_no(FILE *arquivo, int rrn, no_arvore_b *no)
{
    // Posiciona o ponteiro: Tamanho do cabecalho + (RRN * Tamanho do No)
    fseek(arquivo, TAM_CABECALHO + (rrn * TAM_NO), SEEK_SET);

    // Escrita campo a campo obrigatoria conforme as restricoes
    fwrite(&no->removido, sizeof(char), 1, arquivo);
    fwrite(&no->proximo, sizeof(int), 1, arquivo);
    fwrite(&no->tipo_no, sizeof(int), 1, arquivo);
    fwrite(&no->numero_chaves, sizeof(int), 1, arquivo);

    // Chaves e Referencias (Intercalados C1, PR1, C2, PR2...)
    for (int i = 0; i < MAX_CHAVES; i++)
    {
        fwrite(&no->chaves[i], sizeof(int), 1, arquivo);
        fwrite(&no->dados_byte_offsets[i], sizeof(int), 1, arquivo);
    }

    // Ponteiros das Subarvores (P1, P2, P3, P4)
    for (int i = 0; i < ORDEM; i++)
    {
        fwrite(&no->filhos[i], sizeof(int), 1, arquivo);
    }
}

void arvore_b_fechar(FILE *arquivo, header_arvore_b *cabecalho)
{
    if (arquivo == NULL)
        return;

    // Atualiza status para consistente
    cabecalho->status = '1';

    // Reescreve o cabecalho
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&cabecalho->status, sizeof(char), 1, arquivo);
    fwrite(&cabecalho->no_raiz, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->topo, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->prox_RRN, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->nro_nos, sizeof(int), 1, arquivo);

    fclose(arquivo);
}