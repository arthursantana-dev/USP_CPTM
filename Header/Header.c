#include "Header.h"

int escrever_header_no_arquivo(FILE *f_dados, Header *header_dados)
{
    fseek(f_dados, 0, SEEK_SET);
    fwrite(&header_dados->status, sizeof(header_dados->status), 1, f_dados);
    fwrite(&header_dados->topo, sizeof(header_dados->topo), 1, f_dados);
    fwrite(&header_dados->proxRRN, sizeof(header_dados->proxRRN), 1, f_dados);
    fwrite(&header_dados->nroEstacoes, sizeof(header_dados->nroEstacoes), 1, f_dados);
    fwrite(&header_dados->nroParesEstacao, sizeof(header_dados->nroParesEstacao), 1, f_dados);
    return 0;
}

Header *ler_header_do_arquivo(FILE *f_dados)
{
    Header *header_dados = (Header *)malloc(sizeof(Header));
    fseek(f_dados, 0, SEEK_SET);
    if (fread(&header_dados->status, sizeof(header_dados->status), 1, f_dados) != 1)
    {
        return NULL;
    }

    if (header_dados->status == '0')
    {
        free(header_dados);
        return NULL;
    }

    if (fread(&header_dados->topo, sizeof(header_dados->topo), 1, f_dados) != 1)
    {
        return NULL;
    }
    if (fread(&header_dados->proxRRN, sizeof(header_dados->proxRRN), 1, f_dados) != 1)
    {
        return NULL;
    }
    if (fread(&header_dados->nroEstacoes, sizeof(header_dados->nroEstacoes), 1, f_dados) != 1)
    {
        return NULL;
    }
    if (fread(&header_dados->nroParesEstacao, sizeof(header_dados->nroParesEstacao), 1, f_dados) != 1)
    {
        return NULL;
    }

    return header_dados;
}

