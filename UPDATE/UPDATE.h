#ifndef UPDATE_H
    #define UPDATE_H

    #include <stdio.h>
    #include <stdlib.h>

    #include "../utils/utils.h"
    #include "../ParEstacoes/ParEstacoes.h"
    #include "../buffer/buffer.h"
    #include "../Header/Header.h"
    #include "../SetNomes/SetNomes.h"

    #include "../arquivo/arquivo.h"

    int UPDATE(char *nome_arquivo_binario, Estacao *estacao_busca, Estacao *estacao_valores, FILE *f);
#endif