#ifndef ARQUIVO_H
    #define ARQUIVO_H

    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>

    #include "../CONSTS.h"
    #include "../Header/Header.h"
    #include "../Estacao/Estacao.h"

    // leitura dos campos do arquivo .csv
    char *obter_proximo_campo(char **ponteiro_linha);
    
    // CodEstacao,NomeEstacao,CodLinha,NomeLinha,CodProxEst,DistanciaProxEst,CodLinhaInteg,CodEstacaoInteg
    int linha_csv_para_estacao(char *linha_csv, Estacao *estacao);

    void escrever_buffer_no_arquivo(FILE *f, char *buffer);

    int escrever_header_no_arquivo(FILE *f, Header *header);

    Header *ler_header_do_arquivo(FILE *f);

#endif

