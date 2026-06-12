#include "BTREE.h"

/**
 * @brief Função auxiliar interna que executa a busca na Árvore-B de forma recursiva.
 * @param arquivo Ponteiro para o arquivo binário do índice Árvore-B.
 * @param RRN_atual RRN do nó atual a ser inspecionado.
 * @param chave_busca O valor de codEstacao que está sendo procurado.
 * @return Retorna o ponteiro para o arquivo de dados (PR) se encontrado, ou -1 caso contrário.
 */
int arvore_b_buscar_recursivo(FILE *arquivo, int RRN_atual, int chave_busca)
{
    // caso base: se o RRN for -1, atingiu uma subárvore inexistente (chave não encontrada)
    if (RRN_atual == -1)
    {
        return -1;
    }

    // realiza a leitura física do nó atual a partir do disco
    no_arvore_b no = arvore_b_ler_no(arquivo, RRN_atual);

    int i = 0;
    // percorre as chaves do nó enquanto a chave procurada for maior que as chaves locais
    while (i < no.numero_chaves && chave_busca > no.chaves[i])
    {
        i++;
    }

    // caso 1: A chave foi encontrada no nó atual
    if (i < no.numero_chaves && chave_busca == no.chaves[i])
    {
        return no.dados_byte_offsets[i]; // retorna o PR (ponteiro para o arquivo de dados)
    }

    // caso 2: A chave não está neste nó e ele é um nó folha (tipo_no == -1)
    if (no.tipo_no == -1)
    {
        return -1; // não há mais subárvores para descer
    }

    // caso 3: Intermediário/Raiz. Continua a busca descendo para a subárvore correspondente
    return arvore_b_buscar_recursivo(arquivo, no.filhos[i], chave_busca);
}

/**
 * @brief Inicia a busca a partir do nó raiz da Árvore-B de forma gerencial.
 * @param arquivo Ponteiro para o arquivo binário do índice Árvore-B.
 * @param cabecalho Estrutura do cabeçalho da árvore para obter a raiz atual.
 * @param chave_busca O valor de codEstacao que está sendo procurado.
 * @return Retorna o PR se encontrado, ou -1 se a árvore estiver vazia ou não contiver a chave.
 */
int arvore_b_buscar(FILE *arquivo, header_arvore_b *cabecalho, int chave_busca)
{
    if (arquivo == NULL || cabecalho == NULL || cabecalho->no_raiz == -1)
    {
        return -1; // Árvore vazia ou parâmetros inválidos
    }

    // dispara a recursão passando o RRN do nó raiz
    return arvore_b_buscar_recursivo(arquivo, cabecalho->no_raiz, chave_busca);
}

