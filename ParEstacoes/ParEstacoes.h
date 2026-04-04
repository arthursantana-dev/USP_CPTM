#ifndef PARESTACOES_H
#define PARESTACOES_H

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
 * @brief Procura e remove da lista encadeada o par cuja estação de origem corresponda ao código fornecido.
 * @param info_pares_estacoes Ponteiro para a estrutura gestora da lista de pares.
 * @param codEstacaoRemovida Código (ID) da estação de origem que servirá de critério para a remoção.
 */
void destruir_pares_estacoes(InfoParesEstacoes *info_pares_estacoes, int codEstacaoRemovida);

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
 * @brief Ordena a lista encadeada de pares. Transfere os dados para um vetor contíguo, aplica o algoritmo qsort e devolve os valores ordenados à lista.
 * @param info Ponteiro para a estrutura gestora da lista de pares que será ordenada in-place.
 */
void ordenar_pares(InfoParesEstacoes *info);

/**
 * @brief Fornece acesso ao primeiro nó da lista encadeada para permitir iteração externa sem expor diretamente a estrutura interna.
 * @param info Ponteiro para a estrutura gestora da lista de pares.
 * @param par_externo Ponteiro duplo que receberá o endereço do nó inicial (início da lista).
 */
void iniciar_iterador(InfoParesEstacoes *info, ParEstacoes **par_externo);

/**
 * @brief Percorre a lista de pares para verificar se uma determinada ligação (origem -> destino) já se encontra registada.
 * @param info Ponteiro para a estrutura gestora da lista de pares.
 * @param origem Código numérico da estação de origem procurada.
 * @param destino Código numérico da estação de destino procurada.
 * @return Retorna 1 se a correspondência exata do par for encontrada, ou 0 caso contrário.
 */
int existe_par(InfoParesEstacoes *info, int origem, int destino);

#endif