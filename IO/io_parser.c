#include "IO.h"

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

