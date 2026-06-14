#ifndef ESTRUTURAS_AUXILIARES_H
#define ESTRUTURAS_AUXILIARES_H

#include "../Estacao/Estacao.h"
#include "../IO/IO.h"

typedef struct ParEstacoes {
    int origem;
    int destino;
    struct ParEstacoes *prox;
} ParEstacoes;

typedef struct {
    int nroPares;          
    ParEstacoes *inicio;   
} InfoParesEstacoes;

/**
 * @brief Função de comparação utilizada pelo qsort para ordenar os pares de estações.
 * @param a Ponteiro genérico para o primeiro par (ParEstacoes).
 * @param b Ponteiro genérico para o segundo par (ParEstacoes).
 * @return Retorna um valor negativo, zero ou positivo, ordenando primeiramente pela origem e, em caso de empate, pelo destino.
 */
int comparar_pares(const void *a, const void *b);

/**
 * @brief Inicializa os valores de uma estrutura gestora de pares já alocada.
 * @param info Ponteiro para a estrutura InfoParesEstacoes a ser inicializada (define contadores a zero e ponteiros a NULL).
 */
void inicializar_pares(InfoParesEstacoes *info);

/**
 * @brief Cria e insere um novo par (origem -> destino) no início da lista, desde que não seja um destino nulo (-1) e o par ainda não exista.
 * @param info Ponteiro para a estrutura gestora da lista de pares.
 * @param origem Código numérico da estação de origem.
 * @param destino Código numérico da estação de destino.
 */
void inserir_par(InfoParesEstacoes *info, int origem, int destino);

/**
 * @brief Itera pela lista de pares, libertando a memória de todos os nós alocados e reiniciando a estrutura gestora.
 * @param info Ponteiro para a estrutura InfoParesEstacoes a ser esvaziada.
 */
void destruir_pares(InfoParesEstacoes *info);

/**
 * @brief Percorre a lista de pares para verificar se uma determinada ligação (origem -> destino) já se encontra registada.
 * @param info Ponteiro para a estrutura gestora da lista de pares.
 * @param origem Código numérico da estação de origem procurada.
 * @param destino Código numérico da estação de destino procurada.
 * @return Retorna 1 se a correspondência exata do par for encontrada, ou 0 caso contrário.
 */
int existe_par(InfoParesEstacoes *info, int origem, int destino);

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
 * @brief Itera sobre todo o conjunto, libertando a memória de cada string e de cada nó, bem como da própria estrutura principal.
 * @param set Ponteiro para o conjunto de estações a ser destruído.
 * @return Retorna 0 após a conclusão da libertação da memória.
 */
int destruir_set_estacoes(SetNomesEstacoes* set);


#endif