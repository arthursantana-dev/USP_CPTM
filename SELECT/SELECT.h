#ifndef SELECT_H
    #define SELECT_H

    #include <stdio.h>
    #include <stdlib.h>

    #include "../utils/utils.h"
    #include "../ParEstacoes/ParEstacoes.h"
    #include "../buffer/buffer.h"
    #include "../Header/Header.h"
    #include "../arquivo/arquivo.h"
    #include "../lista/lista.h"
    typedef struct _query QUERY;
    void select_all(char *nome_arquivo_binario);
    LISTA* where();
    LISTA* select(LISTA* where, char *nome_arquivo_binario);

#endif