#include "Header.h"

int escrever_header_no_arquivo(FILE *f_dados, Header *header)
{
    fseek(f_dados, 0, SEEK_SET);
    fwrite(&header->status, sizeof(header->status), 1, f_dados);
    fwrite(&header->topo, sizeof(header->topo), 1, f_dados);
    fwrite(&header->proxRRN, sizeof(header->proxRRN), 1, f_dados);
    fwrite(&header->nroEstacoes, sizeof(header->nroEstacoes), 1, f_dados);
    fwrite(&header->nroParesEstacao, sizeof(header->nroParesEstacao), 1, f_dados);
    return 0;
}

Header *ler_header_do_arquivo(FILE *f_dados)
{
    Header *header = (Header *)malloc(sizeof(Header));
    fseek(f_dados, 0, SEEK_SET);
    if (fread(&header->status, sizeof(header->status), 1, f_dados) != 1)
    {
        return NULL;
    }

    if (header->status == '0')
    {
        free(header);
        return NULL;
    }

    if (fread(&header->topo, sizeof(header->topo), 1, f_dados) != 1)
    {
        return NULL;
    }
    if (fread(&header->proxRRN, sizeof(header->proxRRN), 1, f_dados) != 1)
    {
        return NULL;
    }
    if (fread(&header->nroEstacoes, sizeof(header->nroEstacoes), 1, f_dados) != 1)
    {
        return NULL;
    }
    if (fread(&header->nroParesEstacao, sizeof(header->nroParesEstacao), 1, f_dados) != 1)
    {
        return NULL;
    }

    return header;
}

