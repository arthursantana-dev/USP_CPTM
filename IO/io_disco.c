#include <string.h>
#include <stdlib.h>

#include "../CONSTS.h"
#include "../Estacao/Estacao.h"
#include "../ParEstacoes/ParEstacoes.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../Header/Header.h"

/*Escrita*/

void escrever_buffer_no_arquivo(FILE *f, char *buffer)
{
    fwrite(buffer, sizeof(char), TAM_REGISTRO, f);
}

int escrever_header_no_arquivo(FILE *f, Header *header)
{
    fseek(f, 0, SEEK_SET);
    fwrite(&header->status, sizeof(header->status), 1, f);
    fwrite(&header->topo, sizeof(header->topo), 1, f);
    fwrite(&header->proxRRN, sizeof(header->proxRRN), 1, f);
    fwrite(&header->nroEstacoes, sizeof(header->nroEstacoes), 1, f);
    fwrite(&header->nroParesEstacao, sizeof(header->nroParesEstacao), 1, f);
    return 0;
}

/*Leitura*/

Header *ler_header_do_arquivo(FILE *f)
{
    Header *header = (Header *)malloc(sizeof(Header));
    fseek(f, 0, SEEK_SET);
    if (fread(&header->status, sizeof(header->status), 1, f) != 1)
    {
        return NULL;
    }

    if (header->status == '0')
    {
        free(header);
        return NULL;
    }

    if (fread(&header->topo, sizeof(header->topo), 1, f) != 1)
    {
        return NULL;
    }
    if (fread(&header->proxRRN, sizeof(header->proxRRN), 1, f) != 1)
    {
        return NULL;
    }
    if (fread(&header->nroEstacoes, sizeof(header->nroEstacoes), 1, f) != 1)
    {
        return NULL;
    }
    if (fread(&header->nroParesEstacao, sizeof(header->nroParesEstacao), 1, f) != 1)
    {
        return NULL;
    }

    return header;
}
