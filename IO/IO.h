#ifndef IO_H
    #define IO_H

    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <ctype.h>

    #include "../Estacao/Estacao.h"
    #include "../CONSTS.h"

    #include "../Header/Header.h"

    /*Escrita - Console*/

    void imprimir_estacao(Estacao *estacao);

    void mostrar_erro();

    void BinarioNaTela(char *arquivo);

    /*Leitura - Arquivo*/

    int decompor_linha(char *linha, char *vetor[]);

    void vetor_char_para_estacao(Estacao *estacao, char *vetor[], int num_campos);

    char *obter_proximo_campo_csv(char **ponteiro_linha);

    int linha_csv_para_estacao(char *linha_csv, Estacao *estacao);

    /*Leitura - Console*/

    void ler_input_para_estacao_de_busca(Estacao *estacao);

    void input_para_estacao(Estacao *estacao, char *linha);

    /*Miscellaneous - Fornecidas + Multiproposito*/

    void ScanQuoteString(char *str);

    void nullOrInt(int *n);

    void nullOrString(char *str);

#endif