#ifndef CRUD_H
    #define CRUD_H

    #include <stdio.h>
    #include <stdlib.h>

    #include "../EstruturasAuxiliares/EstruturasAuxiliares.h"
    #include "../Header/Header.h"
    #include "../BTREE/BTREE.h"

    int CREATE(char *nome_arquivo_csv, char *nome_arquivo_binario);

    int DELETE(int n, FILE *f_dados, FILE* f_ab);

    int INSERT(FILE *f_dados, FILE *f_ab);

    int SELECT_ALL(FILE *f_dados);

    int SELECT(int n, FILE *f_dados, FILE *f_ab);

    int UPDATE(int n, FILE *f_dados);

    int CREATE_INDEX(char *nome_arquivo_binario, char* nome_arquivo_arvore_b);

#endif