#ifndef ARQUIVO_H
    #define ARQUIVO_H

    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>

    #include "../CONSTS.h"
    #include "../Header/Header.h"
    #include "../Estacao/Estacao.h"

    /**
     * @brief Isola e extrai a próxima string (campo) dividida por delimitador a partir de uma linha crua do formato CSV.
     * @param ponteiro_linha Ponteiro duplo para a string da linha atual (o endereço avançará automaticamente após a leitura do campo).
     * @return Retorna o segmento de string contendo o valor numérico ou texto extraído.
     */
    char *obter_proximo_campo(char **ponteiro_linha);

    /**
     * @brief Realiza o parsing de uma linha inteira lida de um arquivo CSV e a converte em atributos de uma estrutura Estacao.
     * @param linha_csv String bruta representando uma linha completa do CSV.
     * @param estacao Ponteiro para a estrutura Estacao que receberá os campos separados e tipados.
     * @return Retorna um código de status inteiro (0 bem-sucedido, -1 malsucedido).
     */
    int linha_csv_para_estacao(char *linha_csv, Estacao *estacao);

    /**
     * @brief Escreve fisicamente o conteúdo de um array de bytes no arquivo binário aberto, campo a campo.
     * @param f Ponteiro de manipulação do arquivo binário alvo.
     * @param buffer Ponteiro para o array de caracteres que contém a serialização do registro.
     */
    void escrever_buffer_no_arquivo(FILE *f, char *buffer);

    /**
     * @brief Realiza a sobrescrita do registro de cabeçalho no arquivo binário, garantindo o controle de status e ponteiros.
     * @param f Ponteiro de manipulação do arquivo binário alvo.
     * @param header Estrutura com os dados gerenciais (status, topo, etc.) que formam o cabeçalho.
     * @return Retorna um código indicador de sucesso (1) ou falha (0).
     */
    int escrever_header_no_arquivo(FILE *f, Header *header);

    /**
     * @brief Lê sequencialmente o topo do arquivo binário e mapeia seus bytes numa estrutura de Header manipulável. 
     * @param f Ponteiro de manipulação do arquivo binário alvo.
     * @return Retorna o ponteiro para a estrutura do Header instanciada, ou NULL em caso de problemas de I/O.
     */
    Header *ler_header_do_arquivo(FILE *f);

#endif

