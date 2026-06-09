#ifndef CRUD_H
    #define CRUD_H

    #include <stdio.h>
    #include <stdlib.h>

    #include "../EstruturasAuxiliares/EstruturasAuxiliares.h"
    #include "../Header/Header.h"
    #include "../BTREE/BTREE.h"

    int CREATE(char *nome_arquivo_csv, char *nome_arquivo_binario);

    int DELETE(int n, FILE *f, FILE* fab);

    int INSERT(FILE *f, FILE *fab);

    int SELECT_ALL(FILE *f);

    int SELECT(int n, FILE *f, FILE *fab);

    int UPDATE(int n, FILE *f);

    int CREATE_INDEX(char *nome_arquivo_binario, char* nome_arquivo_arvore_b);

#endif