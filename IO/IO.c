#include "../CONSTS.h"
#include "../Estacao/Estacao.h"
#include "../Header/Header.h"

char *criar_buffer()
{
    char *buffer = (char *)malloc(sizeof(char) * TAM_REGISTRO);

    return buffer;
}

int escrever_estacao_no_buffer(Estacao *estacao, char *buffer)
{
    // escrever
    int offset = 0;

    memset(buffer, CHAR_LIXO, TAM_REGISTRO);

    memcpy(buffer + offset, &estacao->removido, sizeof(estacao->removido));
    offset += sizeof(estacao->removido);
    memcpy(buffer + offset, &estacao->proximo, sizeof(estacao->proximo));
    offset += sizeof(estacao->proximo);
    memcpy(buffer + offset, &estacao->codEstacao, sizeof(estacao->codEstacao));
    offset += sizeof(estacao->codEstacao);
    memcpy(buffer + offset, &estacao->codLinha, sizeof(estacao->codLinha));
    offset += sizeof(estacao->codLinha);
    memcpy(buffer + offset, &estacao->codProxEstacao, sizeof(estacao->codProxEstacao));
    offset += sizeof(estacao->codProxEstacao);
    memcpy(buffer + offset, &estacao->distProxEstacao, sizeof(estacao->distProxEstacao));
    offset += sizeof(estacao->distProxEstacao);
    memcpy(buffer + offset, &estacao->codLinhaIntegra, sizeof(estacao->codLinhaIntegra));
    offset += sizeof(estacao->codLinhaIntegra);
    memcpy(buffer + offset, &estacao->codEstacaoIntegra, sizeof(estacao->codEstacaoIntegra));
    offset += sizeof(estacao->codEstacaoIntegra);
    memcpy(buffer + offset, &estacao->tamNomeEstacao, sizeof(estacao->tamNomeEstacao));
    offset += sizeof(estacao->tamNomeEstacao);
    memcpy(buffer + offset, estacao->nomeEstacao, estacao->tamNomeEstacao);
    offset += estacao->tamNomeEstacao;
    memcpy(buffer + offset, &estacao->tamNomeLinha, sizeof(estacao->tamNomeLinha));
    offset += sizeof(estacao->tamNomeLinha);
    memcpy(buffer + offset, estacao->nomeLinha, estacao->tamNomeLinha);
    offset += estacao->tamNomeLinha;

    return offset; // Retorna o número de bytes escritos no buffer
}

int escrever_buffer_na_estacao(char *buffer, Estacao *estacao)
{

    int offset = 0;
    char removido;
    int proximo;
    int codEstacao;
    int codLinha;
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaIntegra;
    int codEstacaoIntegra;
    int tamNomeEstacao;
    int tamNomeLinha;

    memcpy(&removido, buffer + offset, sizeof(removido));
    offset += sizeof(removido);

    memcpy(&proximo, buffer + offset, sizeof(proximo));
    offset += sizeof(proximo);

    memcpy(&codEstacao, buffer + offset, sizeof(codEstacao));
    offset += sizeof(codEstacao);

    memcpy(&codLinha, buffer + offset, sizeof(codLinha));
    offset += sizeof(codLinha);

    memcpy(&codProxEstacao, buffer + offset, sizeof(codProxEstacao));
    offset += sizeof(codProxEstacao);

    memcpy(&distProxEstacao, buffer + offset, sizeof(distProxEstacao));
    offset += sizeof(distProxEstacao);

    memcpy(&codLinhaIntegra, buffer + offset, sizeof(codLinhaIntegra));
    offset += sizeof(codLinhaIntegra);

    memcpy(&codEstacaoIntegra, buffer + offset, sizeof(codEstacaoIntegra));
    offset += sizeof(codEstacaoIntegra);

    memcpy(&tamNomeEstacao, buffer + offset, sizeof(tamNomeEstacao));
    offset += sizeof(tamNomeEstacao);

    estacao->nomeEstacao = (char *)malloc(sizeof(char) * (tamNomeEstacao + 1));

    memcpy(estacao->nomeEstacao, buffer + offset, tamNomeEstacao);

    // nomeEstacao[tamNomeEstacao] = '\0';

    offset += tamNomeEstacao;

    memcpy(&tamNomeLinha, buffer + offset, sizeof(tamNomeLinha));
    offset += sizeof(tamNomeLinha);

    char nomeLinha[tamNomeLinha];

    memcpy(nomeLinha, buffer + offset, tamNomeLinha);

    // nomeLinha[tamNomeLinha] = '\0';

    estacao->nomeEstacao[tamNomeEstacao] = '\0';

    estacao->nomeLinha = (char *)malloc(tamNomeLinha + 1);
    memcpy(estacao->nomeLinha, nomeLinha, tamNomeLinha);
    estacao->nomeLinha[tamNomeLinha] = '\0';

    estacao->removido = removido;
    estacao->proximo = proximo;
    estacao->codEstacao = codEstacao;
    estacao->codLinha = codLinha;
    estacao->codProxEstacao = codProxEstacao;
    estacao->distProxEstacao = distProxEstacao;
    estacao->codLinhaIntegra = codLinhaIntegra;
    estacao->codEstacaoIntegra = codEstacaoIntegra;
    estacao->tamNomeEstacao = tamNomeEstacao;
    estacao->tamNomeLinha = tamNomeLinha;

    return 1;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../CONSTS.h"

#include "../Header/Header.h"
#include "../Estacao/Estacao.h"
#include "../utils/utils.h"

// leitura dos campos do arquivo .csv
char *obter_proximo_campo(char **ponteiro_linha)
{
    char *inicio = *ponteiro_linha;

    if (inicio == NULL)
        return NULL;

    char *virgula = strchr(inicio, ',');

    if (virgula != NULL)
    {
        *virgula = '\0';
        *ponteiro_linha = virgula + 1;
    }
    else
    {
        char *quebra_linha = strpbrk(inicio, "\r\n");
        if (quebra_linha)
            *quebra_linha = '\0';

        *ponteiro_linha = NULL;
    }

    return inicio;
}

// remove caracteres acentuados comuns do UTF-8 e os transforma em ASCII
void normalizar_utf8(char *str)
{
    int i = 0, j = 0;
    while (str[i] != '\0')
    {
        if ((unsigned char)str[i] == 0xC3 && str[i + 1] != '\0')
        {
            i++;
            switch ((unsigned char)str[i])
            {
            case 0xA7:
                str[j++] = 'c';
                break; // ç
            case 0x87:
                str[j++] = 'C';
                break; // Ç
            case 0xA1:
            case 0xA0:
            case 0xA2:
            case 0xA3:
                str[j++] = 'a';
                break; // á, à, â, ã
            case 0x81:
            case 0x80:
            case 0x82:
            case 0x83:
                str[j++] = 'A';
                break; // Á, À, Â, Ã
            case 0xA9:
            case 0xA8:
            case 0xAA:
                str[j++] = 'e';
                break; // é, è, ê
            case 0x89:
            case 0x88:
            case 0x8A:
                str[j++] = 'E';
                break; // É, È, Ê
            case 0xAD:
            case 0xAC:
            case 0xAE:
                str[j++] = 'i';
                break; // í, ì, î
            case 0x8D:
            case 0x8C:
            case 0x8E:
                str[j++] = 'I';
                break; // Í, Ì, Î
            case 0xB3:
            case 0xB2:
            case 0xB4:
            case 0xB5:
                str[j++] = 'o';
                break; // ó, ò, ô, õ
            case 0x93:
            case 0x92:
            case 0x94:
            case 0x95:
                str[j++] = 'O';
                break; // Ó, Ò, Ô, Õ
            case 0xBA:
            case 0xB9:
            case 0xBB:
                str[j++] = 'u';
                break; // ú, ù, û
            case 0x9A:
            case 0x99:
            case 0x9B:
                str[j++] = 'U';
                break; // Ú, Ù, Û
            default:
                str[j++] = '?';
                break;
            }
        }
        else
        {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0';
}

// CodEstacao,NomeEstacao,CodLinha,NomeLinha,CodProxEst,DistanciaProxEst,CodLinhaInteg,CodEstacaoInteg
int linha_csv_para_estacao(char *linha_csv, Estacao *estacao)
{
    char *ponteiro_linha = linha_csv;
    char *token;

    estacao->removido = '0';
    estacao->proximo = -1;

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->codEstacao = atoi(token);

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;

    // normalizar_utf8(token);

    estacao->tamNomeEstacao = strlen(token);
    estacao->nomeEstacao = strdup(token);

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;

    // normalizar_utf8(token);

    estacao->codLinha = atoi(token) == 0 ? -1 : atoi(token);

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->tamNomeLinha = strlen(token);
    estacao->nomeLinha = strdup(token);

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->codProxEstacao = atoi(token) == 0 ? -1 : atoi(token);

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->distProxEstacao = atoi(token) == 0 ? -1 : atoi(token);

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->codLinhaIntegra = atoi(token) == 0 ? -1 : atoi(token);

    token = obter_proximo_campo(&ponteiro_linha);
    if (token == NULL)
        return -1;
    estacao->codEstacaoIntegra = atoi(token) == 0 ? -1 : atoi(token);

    return 0;
}

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

Header *ler_header_do_arquivo(FILE *f)
{
    Header *header = (Header *)malloc(sizeof(Header));
    fseek(f, 0, SEEK_SET);
    if (fread(&header->status, sizeof(header->status), 1, f) != 1)
    {
        mostrar_erro();
        return NULL;
    }

    if (header->status == '0')
    {
        free(header);
        return NULL;
    }

    if (fread(&header->topo, sizeof(header->topo), 1, f) != 1)
    {
        mostrar_erro();
        return NULL;
    }
    if (fread(&header->proxRRN, sizeof(header->proxRRN), 1, f) != 1)
    {
        mostrar_erro();
        return NULL;
    }
    if (fread(&header->nroEstacoes, sizeof(header->nroEstacoes), 1, f) != 1)
    {
        mostrar_erro();
        return NULL;
    }
    if (fread(&header->nroParesEstacao, sizeof(header->nroParesEstacao), 1, f) != 1)
    {
        mostrar_erro();
        return NULL;
    }

    return header;
}