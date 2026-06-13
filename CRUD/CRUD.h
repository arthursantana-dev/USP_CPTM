#ifndef CRUD_H
    #define CRUD_H

    #include <stdio.h>
    #include <stdlib.h>

    #include "../EstruturasAuxiliares/EstruturasAuxiliares.h"
    #include "../Header/Header.h"
    #include "../BTREE/BTREE.h"

    int CREATE(char *nome_arquivo_csv, char *nome_arquivo_binario);

    int DELETE(int n, FILE *f_dados, FILE* f_ab);

    // na especificação, não há menção sobre teste de inserção sem usar a árvore-b (caso Trabalho Introdutorio). entao, modificou-se a função de modo que ela só funcione se um arquivo de indice btree for passado como argumento. caso contrário, retorna falha.
    int INSERT(FILE *f_dados, FILE *f_ab);

    int SELECT_ALL(FILE *f_dados);

    // o mesmo comentário do INSERT se aplica ao SELECT, ou seja, a função só funciona se um arquivo de indice btree for passado como argumento. caso contrário, retorna falha.
    int SELECT(int n, FILE *f_dados, FILE *f_ab);

    int UPDATE(int n, FILE *f_dados);

    int CREATE_INDEX(char *nome_arquivo_binario, char* nome_arquivo_btree);

#endif