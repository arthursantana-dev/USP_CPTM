#include "utils.h"

void mostrar_erro()
{
    printf("Falha no processamento do arquivo.\n");
}

void utils_mostrar_buffer_como_bytes(char *buffer)
{
    for (int i = 0; i < TAM_REGISTRO; i++)
    {
        printf("%02X ", (unsigned char)buffer[i]);
        if ((i + 1) % 4 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}

void utils_imprimir_estacao(Estacao *estacao)
{
    printf("Removido? %s\n", estacao->removido == '1' ? "SIM" : "NÃO");
    printf("RRN do próximo removido: %d\n", estacao->proximo);
    printf("Código da estação: %d\n", estacao->codEstacao);
    printf("Nome da estação: %s\n", estacao->nomeEstacao);
    printf("Código da linha: %d\n", estacao->codLinha);
    printf("Nome da linha: %s\n", estacao->nomeLinha);
    printf("Código da próxima estação: %d\n", estacao->codProxEstacao);
    printf("Distância para a próxima estação: %d\n", estacao->distProxEstacao);
    printf("Código da linha de integração: %d\n", estacao->codLinhaIntegra);
    printf("Código da estação de integração: %d\n", estacao->codEstacaoIntegra);
}

void utils_mostrar_bytes_do_arquivo(FILE *f, int num_bytes)
{
    fseek(f, 0, SEEK_SET);
    unsigned char byte;
    for (int i = 0; i < num_bytes; i++)
    {
        if (fread(&byte, sizeof(byte), 1, f) != 1)
        {
            mostrar_erro();
            return;
        }

        printf("%02X ", byte);
        if ((i + 1) % 4 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}

// quebra a linha em um vetor de strings
int utils_decompor_linha(char *linha, char *vetor[])
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
void utils_vetor_para_estacao(Estacao *estacao, char *vetor[], int num_campos)
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
            if (estacao->nomeEstacao != NULL) {
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
            if (estacao->nomeLinha != NULL) {
                free(estacao->nomeLinha); // Libera o valor anterior ("")
            }
            estacao->tamNomeLinha = strlen(vetor[i + 1]);
            estacao->nomeLinha = (char*) malloc((estacao->tamNomeLinha + 1) * sizeof(char));
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

void utils_linha_para_estacao(Estacao *estacao, char *linha)
{
    char *elementos[MAX_TOKENS];

    int qtd_elementos = utils_decompor_linha(linha, elementos);

    utils_vetor_para_estacao(estacao, elementos, qtd_elementos);
}

int utils_mostrar_pilha_remocao(FILE *f, Header *header)
{
    int rrn = header->topo;

    char buffer[TAM_REGISTRO];

    while (rrn != -1)
    {
        printf("rrn: %d\n", rrn);
        printf("TAM_HEADER + TAM_REGISTRO * rrn: %d\n", TAM_HEADER + TAM_REGISTRO * rrn);

        Estacao *eb = (Estacao *)malloc(sizeof(Estacao));

        fseek(f, TAM_HEADER + TAM_REGISTRO * rrn, SEEK_SET);

        if (fread(buffer, TAM_REGISTRO, 1, f) != 1)
        {
            mostrar_erro();
            free(header);
            free(eb->nomeEstacao);
            free(eb->nomeLinha);
            free(eb);
            return EXIT_FAILURE;
        }

        escrever_buffer_na_estacao(buffer, eb);

        utils_imprimir_estacao(eb);

        printf("--------------\n");

        rrn = eb->proximo;

        free(eb->nomeEstacao);
        free(eb->nomeLinha);
        free(eb);

        // if(j > 3) break;

        // j++;
    }

    return EXIT_SUCCESS;
}

void utils_imprimir_estacao_ln(Estacao *estacao)
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

// formato: nCampos chave1 valor1 chave2 valor2 .. chaveN valorN
void ler_input_para_estacao_de_busca(Estacao *estacao)
{
    int num_campos;
    
    if (scanf("%d", &num_campos) != 1) return;

    char chaves[10][50];   
    char valores[10][256];
    
    char *elementos[20];  
    int qtd_elementos = 0;

    for (int i = 0; i < num_campos; i++)
    {
        scanf("%s", chaves[i]);
        elementos[qtd_elementos++] = chaves[i];

        ScanQuoteString(valores[i]);
        elementos[qtd_elementos++] = valores[i];
    }

    utils_vetor_para_estacao(estacao, elementos, qtd_elementos);
}

// Fornecidas.c

/*
 * Você não precisa entender o código dessa função.
 *
 * Use essa função para comparação no run.codes.
 * Lembre-se de ter fechado (fclose) o arquivo anteriormente.
 *
 * Ela vai abrir de novo para leitura e depois fechar
 * (você não vai perder pontos por isso se usar ela).
 */
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
      // aspas! Fazer leitura normal %s então, pois deve
      // ser algum inteiro ou algo assim...
        str[0] = R;
        scanf("%s", &str[1]);
    }
    else
    { // EOF
        strcpy(str, "");
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
        getchar();       // ignorar o "ULO" de NULO.
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

void nullOrString(char *str)
{
    char R;

    while ((R = getchar()) != EOF && isspace(R))
        ; // ignorar espaços, \r, \n...

    if (R == 'N' || R == 'n')
    { // campo NULO
        getchar();
        getchar();
        getchar();       // ignorar o "ULO" de NULO.
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