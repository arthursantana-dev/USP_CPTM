#ifndef DELETE_H
    #define DELETE_H

    #include <stdio.h>
    #include <stdlib.h>

    #include "../utils/utils.h"
    #include "../arquivo/arquivo.h"
    #include "../buffer/buffer.h"
    #include "../ParEstacoes/ParEstacoes.h"
    #include "../SetNomes/SetNomes.h"

    int deletar_registro(char *nome_arquivo_binario, Estacao *estacao_busca, FILE *f);
#endif