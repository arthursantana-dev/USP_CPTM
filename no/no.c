#include "no.h"
#include <stdio.h>
#include <stdlib.h>

/*Nó é uma estrutura genérica, seu valor pode apontar para qualquer coisa. Os TADs que o manipulam são, portanto, genéricos também. Assim, cada um desses TADs foi "englobado" por um TAD que manipula um tipo específico de dado. Ex: Nó <- lista <- lista_pacientes */
struct No {
    void* valor;
    NO* anterior;
};

NO* no_criar(void* valor, NO* anterior) {
    NO *no = (NO*) malloc(sizeof(NO));
    if (no != NULL) {
        no->valor = valor;
        no->anterior = anterior;
    }
    return no;
}

NO* no_get_anterior(NO* no){
    if(no != NULL){
        return no->anterior;
    }
    return NULL;
}

void no_set_anterior(NO* no, NO* anterior){
    if(no != NULL){
        no->anterior = anterior;
    }
}

void no_remover(NO **no) {
    if (*no != NULL) {
        (*no)->valor = NULL;
        (*no)->anterior = NULL;
        free(*no);
        *no = NULL;
    }
}

/*Método alternativo para remoção, pois um método get do TAD retorna um NO*, não NO**, e usar &no não funcionaria */
void no_remover_ponteiro(NO *no) {
    if (no != NULL) {
        no->valor = NULL;
        no->anterior = NULL;
        free(no);
    }
}

void* no_get_valor(NO* no){

    if(no != NULL){
        return no->valor;
    }

    return NULL;

}

void no_set_valor(NO* no, void* valor){
    if(no != NULL){
        no->valor = valor;
    }
}