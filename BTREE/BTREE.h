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
    int no_raiz;  // RRN do no raiz, -1 se vazia
    int topo;     // RRN de no removido, -1 se nao houver
    int prox_RRN; // proximo RRN livre
    int nro_nos;  // Quantidade de nos na arvore
} header_btree;

/* Estrutura do Registro de Dados / No da Arvore-B (53 bytes) */
typedef struct
{
    char removido;                      // '1' removido, '0' ativo
    int proximo;                        // RRN do proximo no removido
    int tipo_no;                        // -1 folha, 0 raiz, 1 intermediario
    int numero_chaves;                  // Quantidade de chaves no no
    int chaves[MAX_CHAVES];             // C1, C2, C3 (codEstacao)
    int dados_byte_offsets[MAX_CHAVES]; // pR1, PR2, PR3 (RRN no arquivo de dados)
    int filhos[ORDEM];                  // P1, P2, P3, P4 (RRN das subarvores)
} no_btree;

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
} retorno_insercao;

/* =========================================================
 * funcoes de gerenciamento do arquivo
 * ========================================================= */

// inicializa um novo arquivo de indice Arvore-B
FILE *btree_abrir_escrita(const char *nome_arquivo);

// abre um arquivo de indice Arvore-B existente para leitura/escrita
FILE *btree_abrir_leitura(const char *nome_arquivo);

// finaliza o uso do arquivo e atualiza o status para '1'
void btree_fechar(FILE *arquivo, header_btree *cabecalho);

header_btree btree_ler_cabecalho(FILE *arquivo);

void btree_atualizar_cabecalho(FILE *arquivo, header_btree *cabecalho);

/* =========================================================
 * funcoes de manipulacao de nos (escrita/leitura)
 * ========================================================= */

// le um no do disco a partir do seu RRN
no_btree btree_ler_no(FILE *arquivo, int rrn);

// escreve um no no disco em um RRN especifico
void btree_escrever_no(FILE *arquivo, int rrn, no_btree *no);

// instancia um no vazio em memoria com valores iniciais padrao (-1)
no_btree btree_criar_no_vazio(void);

/* =========================================================
 * algoritmos principais (busca, insercao, remocao)
 * ========================================================= */

// busca uma chave na arvore-B. Retorna o PR (referencia do dado) ou -1 se nao encontrar.
int btree_buscar(FILE *arquivo, header_btree *cabecalho, int chave_busca);

// insere uma nova chave e sua referencia na arvore-B, tratando splits e promocoes
void btree_inserir(FILE *arquivo, header_btree *cabecalho, int chave, int byte_offset_dado);

// remove logicamente uma chave, tratando underflows e concatenacoes
void btree_remover(FILE *arquivo, header_btree *cabecalho, int chave_busca);

#endif