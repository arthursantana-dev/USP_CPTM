#ifndef CRUD_H
    #define CRUD_H

    #include <stdio.h>
    #include <stdlib.h>

    #include "../IO/IO.h"

    #include "../utils/utils.h"
    #include "../ParEstacoes/ParEstacoes.h"
    #include "../Header/Header.h"
    #include "../SetNomes/SetNomes.h"

    int CREATE(char *nome_arquivo_csv, char *nome_arquivo_binario);

    /**
     * @brief Realiza a remoção lógica de registros do arquivo binário baseando-se em critérios de busca, inserindo-os na pilha de removidos.
     * @param estacao_busca Estrutura contendo os critérios da busca; registros que baterem com os critérios serão marcados como removidos.
     * @param f Ponteiro para o arquivo binário aberto onde a remoção e alteração do registro ocorrerão.
     * @return Retorna EXIT_SUCCESS após concluir a deleção ou EXIT_FAILURE caso ocorra erro na abertura ou leitura de cabeçalhos.
     */
    int DELETE(Estacao *estacao_busca, FILE* f);

    /**
     * @brief Gerencia o laço de leitura de entradas do usuário e orquestra a inserção de múltiplos registros novos no arquivo binário.
     * @param f Ponteiro para o arquivo binário aberto no modo de atualização.
     * @return Retorna 0 em caso de sucesso total ou 1 em caso de falha (ex: arquivo nulo).
     */
    int INSERT(FILE *f);

    /**
     * @brief Recupera e exibe de forma sequencial os dados de todos os registros existentes no arquivo binário.
     * @param f Ponteiro para o arquivo binário aberto para leitura.
     * @return retorna um inteiro indicando o sucesso (0) ou falha (1) da operação.
     */
    int select_all(FILE *f);

    /**
     * @brief Realiza a busca sequencial de registros no arquivo binário utilizando uma estrutura como "molde" de critérios e imprime os resultados imediatamente na tela ("on the fly"), sem alocação em listas.
     * @param estacao_selecao Ponteiro para a estrutura Estacao preenchida apenas com os valores que devem ser utilizados como critério de busca (filtros).
     * @param f Ponteiro para o arquivo binário de dados já aberto em modo de leitura.
     * @return Retorna EXIT_SUCCESS em caso de sucesso da operação de leitura, ou EXIT_FAILURE caso o arquivo ou cabeçalho sejam inválidos ou nulos.
     */
    int SELECT(Estacao *estacao_selecao, FILE *f);

    /**
     * @brief Atualiza registros no arquivo binário que correspondam aos critérios de busca especificados.
     * @param nome_arquivo_binario String contendo o nome do arquivo binário onde os dados serão atualizados.
     * @param estacao_busca Estrutura contendo os valores que definem o critério de busca (quais registros alterar).
     * @param estacao_valores Estrutura contendo os novos valores que devem ser sobrescritos nos registros encontrados.
     * @return Retorna EXIT_SUCCESS em caso de sucesso ou EXIT_FAILURE em caso de erro.
     */
    int UPDATE(Estacao *estacao_busca, Estacao *estacao_valores, FILE *f);


#endif