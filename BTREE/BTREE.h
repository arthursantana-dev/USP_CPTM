#ifndef ARVORE_B_H
#define ARVORE_B_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "../CONSTS.h"

/* Estrutura do Registro de Cabecalho (17 bytes) */
typedef struct
{
    char status;  // '0' inconsistente, '1' consistente
    int no_raiz;  // rRN do no raiz, -1 se vazia
    int topo;     // rRN de no removido, -1 se nao houver
    int prox_RRN; // Proximo RRN livre
    int nro_nos;  // Quantidade de nos na arvore
} header_arvore_b;

/* Estrutura do Registro de Dados / No da Arvore-B (53 bytes) */
typedef struct
{
    char removido;                      // '1' removido, '0' ativo
    int proximo;                        // rRN do proximo no removido
    int tipo_no;                        // -1 folha, 0 raiz, 1 intermediario
    int numero_chaves;                  // Quantidade de chaves no no
    int chaves[MAX_CHAVES];             // C1, C2, C3 (codEstacao)
    int dados_byte_offsets[MAX_CHAVES]; // PR1, PR2, PR3 (RRN no arquivo de dados)
    int filhos[ORDEM];                  // P1, P2, P3, P4 (RRN das subarvores)
} no_arvore_b;

// estrutura de retorno para guiar a subida recursiva do split
// @param houve_split Indica se ocorreu um split no nível inferior
// @param chave_promovida A chave que deve ser promovida para o nível superior
// @param RRN_dado_promovido O PR associado à chave promovida (relevante para folhas)
// @param RRN_filho_direito O RRN do novo nó criado à direita após o split
typedef struct
{
    bool houve_split;
    int chave_promovida;
    int RRN_dado_promovido;
    int RRN_filho_direito;
} retorno_insercao_t;

/* =========================================================
 * funcoes de gerenciamento do arquivo
 * ========================================================= */

// inicializa um novo arquivo de indice Arvore-B
FILE *arvore_b_abrir_escrita(const char *nome_arquivo);

// abre um arquivo de indice Arvore-B existente para leitura/escrita
FILE *arvore_b_abrir_leitura(const char *nome_arquivo);

// finaliza o uso do arquivo e atualiza o status para '1'
void arvore_b_fechar(FILE *arquivo, header_arvore_b *cabecalho);

header_arvore_b arvore_b_ler_cabecalho(FILE *arquivo);

void arvore_b_atualizar_cabecalho(FILE *arquivo, header_arvore_b *cabecalho);

/* =========================================================
 * funcoes de Manipulacao de Nos (I/O)
 * ========================================================= */

// le um no do disco a partir do seu RRN
no_arvore_b arvore_b_ler_no(FILE *arquivo, int rrn);

// escreve um no no disco em um RRN especifico
void arvore_b_escrever_no(FILE *arquivo, int rrn, no_arvore_b *no);

// instancia um no vazio em memoria com valores iniciais padrao (-1)
no_arvore_b arvore_b_criar_no_vazio(void);

/* =========================================================
 * algoritmos principais (busca, insercao, remocao)
 * ========================================================= */

// busca uma chave na arvore-B. Retorna o PR (referencia do dado) ou -1 se nao encontrar.
int arvore_b_buscar(FILE *arquivo, header_arvore_b *cabecalho, int chave_busca);

// insere uma nova chave e sua referencia na arvore-B, tratando splits e promocoes
void arvore_b_inserir(FILE *arquivo, header_arvore_b *cabecalho, int chave, int byte_offset_dado);

// remove logicamente uma chave, tratando underflows e concatenacoes
void arvore_b_remover(FILE *arquivo, header_arvore_b *cabecalho, int chave_busca);

#endif