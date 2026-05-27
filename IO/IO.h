#ifndef IO_H
    #define IO_H

    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <ctype.h>

    #include "../Estacao/Estacao.h"
    #include "../CONSTS.h"

    #include "../Header/Header.h"


    /*Buffer*/

    /**
	 * @brief Aloca dinamicamente o espaço base na memória para o tamanho exato de um registro fixo do projeto (80 bytes). 
	 * @return Retorna um array de caracteres alocado pronto para uso em leituras ou escritas.
	 */
	char *criar_buffer();
  
    /**
	 * @brief Serializa os dados literais da estrutura Estacao em uma cadeia de bytes fixos, substituindo o espaço sobressalente por lixo de memória ('$').
	 * @param estacao Ponteiro para a estrutura da qual os dados serão extraídos.
	 * @param buffer Ponteiro para o array pré-alocado (buffer) onde os bytes serão assinalados na devida ordem e padding.
	 * @return Retorna a quantidade de bytes total percorrida e inserida (offset final).
	 */
	int escrever_estacao_no_buffer(Estacao *estacao, char *buffer);

	/**
	 * @brief Processo reverso: decodifica um array de bytes contínuo do arquivo transcrevendo as seções binárias de volta para a estrutura Estacao em memória.
	 * @param buffer Ponteiro para a cadeia de bytes puramente lida pelo "fread" de um registro de 80 bytes.
	 * @param estacao Estrutura recipiente que vai alocar e segmentar os dados desserializados.
	 * @return Retorna 1 se o parser do buffer funcionou sem intercorrências, ou 0 em caso de erro.
	 */
	int escrever_buffer_na_estacao(char* buffer, Estacao *estacao);

    /*Disco*/

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

    /*Console*/

    /**
     * @brief Isola e extrai a próxima string (campo) dividida por delimitador a partir de uma linha crua do formato CSV.
     * @param ponteiro_linha Ponteiro duplo para a string da linha atual (o endereço avançará automaticamente após a leitura do campo).
     * @return Retorna o segmento de string contendo o valor numérico ou texto extraído.
     */
    char *obter_proximo_campo(char **ponteiro_linha);

    void ler_input_para_estacao_de_busca(Estacao *estacao);

    void mostrar_erro();

    /*Parser*/

    /**
     * @brief Realiza o parsing de uma linha inteira lida de um arquivo CSV e a converte em atributos de uma estrutura Estacao.
     * @param linha_csv String bruta representando uma linha completa do CSV.
     * @param estacao Ponteiro para a estrutura Estacao que receberá os campos separados e tipados.
     * @return Retorna um código de status inteiro (0 bem-sucedido, -1 malsucedido).
     */
    int linha_csv_para_estacao(char *linha_csv, Estacao *estacao);

	void input_para_estacao(Estacao *estacao, char *linha);

    /*Fornecidas*/

    void BinarioNaTela(char *arquivo);

    void ScanQuoteString(char *str);

    void nullOrInt(int *n);

    void nullOrString(char *str);

#endif