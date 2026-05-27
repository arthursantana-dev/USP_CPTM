#include "IO.h"

void mostrar_erro()
{
    printf("Falha no processamento do arquivo.\n");
}

// quebra a linha em um vetor de strings
int decompor_linha(char *linha, char *vetor[])
{
    int contador = 0;
    char *ptr = linha;

    while (*ptr != '\0')
    {
        while (isspace((unsigned char)*ptr))
        {
            ptr++;
        }

        if (*ptr == '\0')
            break;

        if (*ptr == '"')
        {
            ptr++;
            vetor[contador++] = ptr;

            while (*ptr != '"' && *ptr != '\0')
            {
                ptr++;
            }

            if (*ptr == '"')
            {
                *ptr = '\0';
                ptr++;
            }
        }
        else
        {
            vetor[contador++] = ptr;

            while (!isspace((unsigned char)*ptr) && *ptr != '\0')
            {
                ptr++;
            }

            if (*ptr != '\0')
            {
                *ptr = '\0';
                ptr++;
            }
        }
    }

    return contador;
}

// nomeEstacao e nomeLinha alocados "na hora"
void vetor_char_para_estacao(Estacao *estacao, char *vetor[], int num_campos)
{
    int i = 0;

    if (isdigit(vetor[0][0]))
    {
        i++;
    }

    for (; i < num_campos; i++)
    {
        if (strcmp(vetor[i], "codEstacao") == 0)
        {
            estacao->codEstacao = atoi(vetor[i + 1]);
        }
        else if (strcmp(vetor[i], "nomeEstacao") == 0)
        {
            if (estacao->nomeEstacao != NULL)
            {
                free(estacao->nomeEstacao);
            }
            estacao->tamNomeEstacao = strlen(vetor[i + 1]);
            estacao->nomeEstacao = (char *)malloc((estacao->tamNomeEstacao + 1) * sizeof(char));

            strcpy(estacao->nomeEstacao, vetor[i + 1]);
        }
        else if (strcmp(vetor[i], "codLinha") == 0)
        {
            estacao->codLinha = atoi(vetor[i + 1]);
        }
        else if (strcmp(vetor[i], "nomeLinha") == 0)
        {
            if (estacao->nomeLinha != NULL)
            {
                free(estacao->nomeLinha); // Libera o valor anterior ("")
            }
            estacao->tamNomeLinha = strlen(vetor[i + 1]);
            estacao->nomeLinha = (char *)malloc((estacao->tamNomeLinha + 1) * sizeof(char));
            strcpy(estacao->nomeLinha, vetor[i + 1]);
        }
        else if (strcmp(vetor[i], "codProxEstacao") == 0)
        {
            estacao->codProxEstacao = atoi(vetor[i + 1]);
        }
        else if (strcmp(vetor[i], "distProxEstacao") == 0)
        {
            estacao->distProxEstacao = atoi(vetor[i + 1]);
        }
        else if (strcmp(vetor[i], "codLinhaIntegra") == 0)
        {
            estacao->codLinhaIntegra = atoi(vetor[i + 1]);
        }
        else if (strcmp(vetor[i], "codEstIntegra") == 0)
        {
            estacao->codEstacaoIntegra = atoi(vetor[i + 1]);
        }
    }
}

void input_para_estacao(Estacao *estacao, char *linha)
{
    char *elementos[MAX_TOKENS];

    int qtd_elementos = decompor_linha(linha, elementos);

    vetor_char_para_estacao(estacao, elementos, qtd_elementos);
}

void ler_input_para_estacao_de_busca(Estacao *estacao)
{
    int num_campos;

    if (scanf("%d", &num_campos) != 1)
        return;

    char chaves[10][50];
    char valores[10][256];

    char *elementos[20];
    int qtd_elementos = 0;

    for (int i = 0; i < num_campos; i++)
    {
        scanf("%s", chaves[i]);
        elementos[qtd_elementos++] = chaves[i];

        ScanQuoteString(valores[i]);
        if (strcmp("", valores[i]) == 0)
        {
            memcpy(valores[i], "-1", sizeof("-1"));
        }
        elementos[qtd_elementos++] = valores[i];
    }

    vetor_char_para_estacao(estacao, elementos, qtd_elementos);
}

void imprimir_estacao(Estacao *estacao)
{
    printf("%d ", estacao->codEstacao);

    if (estacao->nomeEstacao != NULL && estacao->tamNomeEstacao > 0)
        printf("%s ", estacao->nomeEstacao);
    else
        printf("NULO ");

    if (estacao->codLinha != -1)
        printf("%d ", estacao->codLinha);
    else
        printf("NULO ");

    if (estacao->nomeLinha != NULL && estacao->tamNomeLinha > 0)
        printf("%s ", estacao->nomeLinha);
    else
        printf("NULO ");

    if (estacao->codProxEstacao != -1)
        printf("%d ", estacao->codProxEstacao);
    else
        printf("NULO ");

    if (estacao->distProxEstacao != -1)
        printf("%d ", estacao->distProxEstacao);
    else
        printf("NULO ");

    if (estacao->codLinhaIntegra != -1)
        printf("%d ", estacao->codLinhaIntegra);
    else
        printf("NULO ");

    if (estacao->codEstacaoIntegra != -1)
        printf("%d ", estacao->codEstacaoIntegra);
    else
        printf("NULO ");

    printf("\n");
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