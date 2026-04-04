#ifndef SET_NOMES_ESTACOES_H
#define SET_NOMES_ESTACOES_H

#include "FrequenciaNomes.h"
#include "../buffer/buffer.h"
#include "../Estacao/Estacao.h"

typedef struct no_set {
    char *nome;
    struct no_set *prox;
} NoSet;

typedef struct {
    NoSet *inicio;
    int tamanho;
} SetNomesEstacoes;

/**
 * @brief Cria e inicializa dinamicamente uma nova estrutura de conjunto (set) de nomes de estações.
 * @return Retorna um ponteiro para a estrutura SetNomesEstacoes instanciada, ou NULL em caso de falha na alocação.
 */
SetNomesEstacoes* criar_set_estacoes();

/**
 * @brief Verifica sequencialmente se o nome de uma estação já existe no conjunto.
 * @param set Ponteiro para o conjunto de estações onde a procura será realizada.
 * @param nome String contendo o nome da estação a verificar.
 * @return Retorna 1 se a estação for encontrada no conjunto, ou 0 caso contrário (ou se os parâmetros forem nulos).
 */
int existe_estacao(SetNomesEstacoes* set, const char* nome);

/**
 * @brief Insere o nome de uma nova estação no início da lista encadeada (O(1)), garantindo que não há duplicados.
 * @param set Ponteiro para o conjunto de estações.
 * @param nome String contendo o nome da estação a ser adicionada.
 * @return Retorna 1 em caso de inserção com sucesso, ou 0 se a estação já existir ou ocorrer uma falha de memória.
 */
int incluir_estacao(SetNomesEstacoes* set, const char* nome);

/**
 * @brief Procura e remove o nó correspondente a uma estação específica, refazendo as ligações da lista e libertando a memória.
 * @param set Ponteiro para o conjunto de estações.
 * @param nome String contendo o nome da estação a ser removida.
 * @return Retorna 1 se a estação for removida com sucesso, ou 0 se a estação não for encontrada.
 */
int remover_estacao(SetNomesEstacoes* set, const char* nome);

/**
 * @brief Itera sobre todo o conjunto, libertando a memória de cada string e de cada nó, bem como da própria estrutura principal.
 * @param set Ponteiro para o conjunto de estações a ser destruído.
 * @return Retorna 0 após a conclusão da libertação da memória.
 */
int destruir_set_estacoes(SetNomesEstacoes* set);

/**
 * @brief Lê o ficheiro binário de dados na sua totalidade para reconstruir um conjunto completo de nomes de estações ativas (não removidas).
 * @param f Ponteiro para o ficheiro binário de onde os registos serão lidos.
 * @return Retorna um ponteiro para o conjunto (SetNomesEstacoes) totalmente populado com os dados do ficheiro.
 */
SetNomesEstacoes* criar_set_estacoes_populado(FILE* f);

#endif