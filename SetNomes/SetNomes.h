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

// cria e inicializa um novo set de estações
SetNomesEstacoes* criar_set_estacoes();

// verifica se uma estação existe no set (retorna 1 se existir, 0 caso contrário)
int existe_estacao(SetNomesEstacoes* set, const char* nome);

// inclui uma nova estação no set (retorna 1 se sucesso, 0 se já existir ou erro)
int incluir_estacao(SetNomesEstacoes* set, const char* nome);

// Remove uma estação do set (retorna 1 se removido com sucesso, 0 se não existir ou erro)
int remover_estacao(SetNomesEstacoes* set, const char* nome);

int destruir_set_estacoes(SetNomesEstacoes* set);

SetNomesEstacoes* criar_set_estacoes_populado(FILE* f);

#endif