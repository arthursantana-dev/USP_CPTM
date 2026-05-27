#ifndef CRUD_H
    #define CRUD_H

    #include <stdio.h>
    #include <stdlib.h>

    #include "../ParEstacoes/ParEstacoes.h"
    #include "../Header/Header.h"
    #include "../SetNomes/SetNomes.h"

    int CREATE(char *nome_arquivo_csv, char *nome_arquivo_binario);

    int DELETE(int n, FILE *f);

    int INSERT(FILE *f);

    int SELECT_ALL(FILE *f);

    int SELECT(int n, FILE *f);

    int UPDATE(int n, FILE *f);

#endif