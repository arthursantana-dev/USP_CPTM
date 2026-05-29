#ifndef HEADER_H
    #define HEADER_H

    #include <stdio.h>
    #include <stdlib.h>

    typedef struct
    {
        char status;		 // '0' para inconsistente, '1' para consistente
        int topo;			 // RRN do topo da pilha de registros removidos. iniciado com -1
        int proxRRN;		 // RRN do próximo registro a ser inserido
        int nroEstacoes;	 // Número de estações cadastradas
        int nroParesEstacao; // Número de pares de estações cadastrados
    } Header;

    int escrever_header_no_arquivo(FILE *f, Header *header);

    Header *ler_header_do_arquivo(FILE *f);

#endif