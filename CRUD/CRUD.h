#ifndef CRUD_H
    #define CRUD_H

    #include <stdio.h>
    #include <stdlib.h>

    #include "../EstruturasAuxiliares/EstruturasAuxiliares.h"
    #include "../Header/Header.h"
    #include "../BTREE/BTREE.h"

    /* funções do trabalho 2 */

    int ORDER_BY (FILE *f_dados, char* campo_ordenacao, char* nome_arquivo_ordenado);

    int SORT_MERGE_JOIN(char* nome_arquivo_binario_A, char* nome_arquivo_binario_B);

    /* funções do trabalho 1 */

    int DELETE(int n, FILE *f_dados, FILE* f_ab);

    int INSERT(FILE *f_dados, FILE *f_ab);

    int SELECT_ALL(FILE *f_dados);

    // o mesmo comentário do INSERT se aplica ao SELECT, ou seja, a função só funciona se um arquivo de indice btree for passado como argumento. caso contrário, retorna falha.
    int SELECT(int n, FILE *f_dados, FILE *f_ab);

    int UPDATE(int n, FILE *f_dados);

    int CREATE_INDEX(char *nome_arquivo_binario, char* nome_arquivo_btree);

    /* funções do trabalho introdutorio */

    int CREATE(char *nome_arquivo_csv, char *nome_arquivo_binario);

    int SELECT_SEM_INDICE(int n, FILE *f_dados);

    int DELETE_SEM_INDICE(int n, FILE *f_dados);

    int INSERT_SEM_INDICE(FILE *f_dados);

#endif