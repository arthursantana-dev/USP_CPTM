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
    /**
     * @brief Atualiza registros no arquivo binário que correspondam aos critérios de busca especificados.
     * @param nome_arquivo_binario String contendo o nome do arquivo binário onde os dados serão atualizados.
     * @param estacao_busca Estrutura contendo os valores que definem o critério de busca (quais registros alterar).
     * @param estacao_valores Estrutura contendo os novos valores que devem ser sobrescritos nos registros encontrados.
     * @return Retorna EXIT_SUCCESS em caso de sucesso ou EXIT_FAILURE em caso de erro.
     */
    int UPDATE(Estacao *estacao_busca, Estacao *estacao_valores, FILE *f);
    
#endif