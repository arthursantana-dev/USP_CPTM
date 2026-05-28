#include <stdio.h>
#include <stdlib.h>

#include "IO.h"

void mostrar_erro()
{
    printf("Falha no processamento do arquivo.\n");
}

void BinarioNaTela(char *arquivo)
{
    FILE *fs;
    if (arquivo == NULL || !(fs = fopen(arquivo, "rb")))
    {
        fprintf(stderr,
                "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): "
                "não foi possível abrir o arquivo que me passou para leitura. "
                "Ele existe e você tá passando o nome certo? Você lembrou de "
                "fechar ele com fclose depois de usar?\n");
        return;
    }

    fseek(fs, 0, SEEK_END);
    size_t fl = ftell(fs);

    fseek(fs, 0, SEEK_SET);
    unsigned char *mb = (unsigned char *)malloc(fl);
    fread(mb, 1, fl, fs);

    unsigned long cs = 0;
    for (unsigned long i = 0; i < fl; i++)
    {
        cs += (unsigned long)mb[i];
    }

    printf("%lf\n", (cs / (double)100));

    free(mb);
    fclose(fs);
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

/*
 *	Use essa função para ler um campo string delimitado entre aspas (").
 *	Chame ela na hora que for ler tal campo. Por exemplo:
 *
 *	A entrada está da seguinte forma:
 *		nomeDoCampo "MARIA DA SILVA"
 *
 *	Para ler isso para as strings já alocadas str1 e str2 do seu programa,
 * você faz: scanf("%s", str1); // Vai salvar nomeDoCampo em str1
 *		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2
 * (sem as aspas)
 *
 */
void ScanQuoteString(char *str)
{
    char R;

    while ((R = getchar()) != EOF && isspace(R))
        ; // ignorar espaços, \r, \n...

    if (R == 'N' || R == 'n')
    { // campo NULO
        getchar();
        getchar();
        getchar();       // ignorar o "ULO" de NULO.
        strcpy(str, ""); // copia string vazia
    }
    else if (R == '\"')
    {
        if (scanf("%[^\"]", str) != 1)
        { // ler até o fechamento das aspas
            strcpy(str, "");
        }
        getchar(); // ignorar aspas fechando
    }
    else if (R != EOF)
    { // vc tá tentando ler uma string que não tá entre
      // aspas! (ex: um número inteiro)
        str[0] = R;
        int i = 1;
        // lê os próximos caracteres até achar um espaço ou \n
        while ((R = getchar()) != EOF && !isspace(R))
        {
            str[i++] = R;
        }
        str[i] = '\0'; // finaliza a string

        // devolve o espaço ou \n para o buffer, para não quebrar a próxima leitura
        if (R != EOF)
        {
            ungetc(R, stdin);
        }
    }
}

void nullOrInt(int *n)
{
    char R;

    while ((R = getchar()) != EOF && isspace(R))
        ; // ignorar espaços, \r, \n...

    if (R == 'N' || R == 'n')
    { // campo NULO
        getchar();
        getchar();
        getchar(); // ignorar o "ULO" de NULO.
        *n = -1;
    }
    else if (R != EOF)
    { // vc tá tentando ler uma string que não tá entre
      // aspas! Fazer leitura normal %s então, pois deve
      // ser algum inteiro ou algo assim...
        ungetc(R, stdin);
        scanf("%d", n);
    }
    else
    { // EOF
        *n = -1;
    }
}
/*
 * Você não precisa entender o código dessa função.
 *
 * Use essa função para comparação no run.codes.
 * Lembre-se de ter fechado (fclose) o arquivo anteriormente.
 *
 * Ela vai abrir de novo para leitura e depois fechar
 * (você não vai perder pontos por isso se usar ela).
 */
void nullOrString(char *str){
    char R;

    while ((R = getchar()) != EOF && isspace(R))
        ; // ignorar espaços, \r, \n...

    if (R == 'N' || R == 'n')
    { // campo NULO
        getchar();
        getchar();
        getchar();         // ignorar o "ULO" de NULO.
        strcpy(str, "-1"); // copia string vazia
    }
    else if (R == '\"')
    {
        if (scanf("%[^\"]", str) != 1)
        { // ler até o fechamento das aspas
            strcpy(str, "-1");
        }
        getchar(); // ignorar aspas fechando
    }
    else if (R != EOF)
    { // vc tá tentando ler uma string que não tá entre
      // aspas! Fazer leitura normal %s então, pois deve
      // ser algum inteiro ou algo assim...
        ungetc(R, stdin);
        scanf("%s", str);
    }
    else
    { // EOF
        strcpy(str, "-1");
    }
}