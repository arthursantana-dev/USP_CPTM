#ifndef CREATE_H
    #define CREATE_H

    #include <stdio.h>
    #include <stdlib.h>

    #include "../utils/utils.h"
    #include "../ParEstacoes/ParEstacoes.h"
    #include "../buffer/buffer.h"
    #include "../Header/Header.h"

    #include "../arquivo/arquivo.h"

    int criar_arquivo_binario(const char *nome_arquivo_csv, const char *nome_arquivo_binario);

#endif