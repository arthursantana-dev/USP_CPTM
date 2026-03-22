#ifndef HEADER_H
    #define HEADER_H

    typedef struct
    {
        char status;		 // '0' para inconsistente, '1' para consistente
        int topo;			 // RRN do topo da pilha de registros removidos. iniciado com -1
        int proxRRN;		 // RRN do próximo registro a ser inserido
        int nroEstacoes;	 // Número de estações cadastradas
        int nroParesEstacao; // Número de pares de estações cadastrados
    } Header;
#endif