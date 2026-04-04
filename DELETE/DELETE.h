#ifndef DELETE_H
    #define DELETE_H

    #include <stdio.h>
    #include <stdlib.h>

    #include "../utils/utils.h"
    #include "../arquivo/arquivo.h"
    #include "../buffer/buffer.h"
    #include "../ParEstacoes/ParEstacoes.h"
    #include "../SetNomes/SetNomes.h"

    /**
     * @brief Realiza a remoção lógica de registros do arquivo binário baseando-se em critérios de busca, inserindo-os na pilha de removidos.
     * @param estacao_busca Estrutura contendo os critérios da busca; registros que baterem com os critérios serão marcados como removidos.
     * @param f Ponteiro para o arquivo binário aberto onde a remoção e alteração do registro ocorrerão.
     * @return Retorna EXIT_SUCCESS após concluir a deleção ou EXIT_FAILURE caso ocorra erro na abertura ou leitura de cabeçalhos.
     */
    int DELETE(Estacao *estacao_busca, FILE* f);
#endif