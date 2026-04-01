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

int comparar_pares(const void *a, const void *b);

void destruir_pares_estacoes(InfoParesEstacoes *info_pares_estacoes, int codEstacaoRemovida);

void inicializar_pares(InfoParesEstacoes *info);

void inserir_par(InfoParesEstacoes *info, int origem, int destino);

void destruir_pares(InfoParesEstacoes *info);

void ordenar_pares(InfoParesEstacoes *info);

void iniciar_iterador(InfoParesEstacoes *info, ParEstacoes **par_externo);

int existe_par(InfoParesEstacoes *info, int origem, int destino);

#endif