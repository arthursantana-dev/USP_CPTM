#include "../ParEstacoes/ParEstacoes.h"
#include "../SetNomes/SetNomes.h"

int CREATE(char *nome_arquivo_csv, char *nome_arquivo_binario)
{

    char *buffer = criar_buffer();

    FILE *csv = fopen(nome_arquivo_csv, "r");
    if (csv == NULL)
    {
        mostrar_erro();
        return EXIT_FAILURE;
    }

    char cabecalho[256];
    fgets(cabecalho, sizeof(cabecalho), csv);

    FILE *bin = fopen(nome_arquivo_binario, "wb+");
    if (bin == NULL)
    {
        free(buffer);
        mostrar_erro();
        fclose(csv);
        return EXIT_FAILURE;
    }

    SetNomesEstacoes *set_estacoes = criar_set_estacoes();

    // estação inconsistente: status = '0' no header, e só é setado para '1' ao final da criação do arquivo
    Header header = {'0', -1, 0, 0, 0};

    escrever_header_no_arquivo(bin, &header);

    InfoParesEstacoes info_pares_estacoes;
    inicializar_pares(&info_pares_estacoes);

    char linha[256];

    int contador_estacoes = 0;

    while (fgets(linha, sizeof(linha), csv))
    {
        Estacao *estacao = (Estacao *)malloc(sizeof(Estacao));
        if (estacao == NULL)
        {
            free(buffer);
            destruir_pares(&info_pares_estacoes);

            destruir_set_estacoes(set_estacoes);
            free(set_estacoes);

            mostrar_erro();
            fclose(csv);
            return EXIT_FAILURE;
        }

        int err = linha_csv_para_estacao(linha, estacao);
        if (err != 0)
        {
            free(estacao->nomeEstacao);
            free(estacao->nomeLinha);
            free(estacao);

            free(buffer);
            destruir_pares(&info_pares_estacoes);

            destruir_set_estacoes(set_estacoes);
            free(set_estacoes);
            mostrar_erro();
            fclose(csv);
            return EXIT_FAILURE;
        }

        escrever_estacao_no_buffer(estacao, buffer);

        if (!existe_estacao(set_estacoes, estacao->nomeEstacao))
        {
            incluir_estacao(set_estacoes, estacao->nomeEstacao);
        }

        escrever_buffer_no_arquivo(bin, buffer);

        if (estacao->codProxEstacao != -1)
        {
            inserir_par(&info_pares_estacoes, estacao->codEstacao, estacao->codProxEstacao);
        }

        contador_estacoes++;

        free(estacao->nomeEstacao);
        free(estacao->nomeLinha);
        free(estacao);
    }

    fseek(bin, 0, SEEK_SET);
    header.status = '1';
    header.nroParesEstacao = info_pares_estacoes.nroPares;
    header.proxRRN = contador_estacoes;
    header.nroEstacoes = set_estacoes->tamanho;

    escrever_header_no_arquivo(bin, &header);

    destruir_pares(&info_pares_estacoes);

    destruir_set_estacoes(set_estacoes);

    free(buffer);

    fclose(bin);

    BinarioNaTela(nome_arquivo_binario);

    return EXIT_SUCCESS;
}

int DELETE(Estacao *estacao_busca, FILE *f)
{

    if (f == NULL)
    {
        mostrar_erro();
        return EXIT_FAILURE;
    }

    Header *header = ler_header_do_arquivo(f);

    if (header == NULL)
    {
        fclose(f);
        return EXIT_FAILURE;
    }

    char buffer[TAM_REGISTRO];

    int removeu_estacao = 0;

    SetNomesEstacoes *set_estacoes = criar_set_estacoes();

    InfoParesEstacoes info_pares_estacoes;
    inicializar_pares(&info_pares_estacoes);

    header->status = '0';
    escrever_header_no_arquivo(f, header);

    int RRNnovo = 0;

    fseek(f, TAM_HEADER, SEEK_SET);

    int rrn_atual = -1;

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));

        escrever_buffer_na_estacao(buffer, ea);

        rrn_atual++;

        if (ea->removido == '1')
        {
            destruir_estacao(ea);
            continue;
        }

        if (!comparar_estacoes(estacao_busca, ea) && ea->codProxEstacao != -1)
        {
            inserir_par(&info_pares_estacoes, ea->codEstacao, ea->codProxEstacao);
        }

        if (!comparar_estacoes(estacao_busca, ea))
        {
            destruir_estacao(ea);
            continue;
        }

        removeu_estacao = 1;

        RRNnovo = rrn_atual;

        ea->removido = '1';
        ea->proximo = header->topo;

        header->topo = RRNnovo;

        escrever_estacao_no_buffer(ea, buffer);

        fseek(f, TAM_HEADER + TAM_REGISTRO * RRNnovo, SEEK_SET);

        escrever_buffer_no_arquivo(f, buffer);

        destruir_estacao(ea);

        fseek(f, 0, SEEK_CUR);
    }

    if (!removeu_estacao)
    {
        header->status = '1';
        escrever_header_no_arquivo(f, header);
        free(header);
        destruir_set_estacoes(set_estacoes);
        destruir_pares(&info_pares_estacoes);
        return EXIT_SUCCESS;
    }

    fseek(f, TAM_HEADER, SEEK_SET);

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));

        escrever_buffer_na_estacao(buffer, ea);

        if (ea->removido == '1')
        {
            destruir_estacao(ea);
            continue;
        }

        incluir_estacao(set_estacoes, ea->nomeEstacao);
        destruir_estacao(ea);
    }

    header->status = '1';
    header->nroEstacoes = set_estacoes->tamanho;
    header->nroParesEstacao = info_pares_estacoes.nroPares;

    escrever_header_no_arquivo(f, header);

    destruir_set_estacoes(set_estacoes);
    destruir_pares(&info_pares_estacoes);

    free(header);

    return EXIT_SUCCESS;
}

int _insert(FILE *f, Estacao *estacao)
{
    // Algoritmo da professora
    Header *header = ler_header_do_arquivo(f);
    if (header == NULL)
        return 1;

    header->status = '0';
    escrever_header_no_arquivo(f, header);
    int topo = header->topo;
    int proxRRN = header->proxRRN;
    char *buffer = criar_buffer();
    if (topo == -1)
    {
        int offset = proxRRN * TAM_REGISTRO + TAM_HEADER;
        fseek(f, offset, SEEK_SET);
        header->proxRRN++;
    }
    else
    {
        int offset = topo * TAM_REGISTRO + TAM_HEADER;
        fseek(f, offset, SEEK_SET);
        fread(buffer, TAM_REGISTRO, 1, f);
        fseek(f, offset, SEEK_SET);
        Estacao *estacao_removida = (Estacao *)calloc(1, sizeof(Estacao));
        escrever_buffer_na_estacao(buffer, estacao_removida);
        header->topo = estacao_removida->proximo;
        destruir_estacao(estacao_removida);
    }
    escrever_estacao_no_buffer(estacao, buffer);
    escrever_buffer_no_arquivo(f, buffer);

    if (estacao->codProxEstacao != -1)
        // Se a estação tem uma próxima estação, então tem um par
        header->nroParesEstacao++;

    header->status = '1';
    escrever_header_no_arquivo(f, header);

    free(buffer);
    free(header);

    return 0;
}


int INSERT(FILE *f)
{
    if (f == NULL)
        return 1;

    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        int codEstacao, codLinha, codProxEstacao, distProxEstacao, codLinhaIntegra, codEstacaoIntegra;
        char *nomeEstacao = (char *)malloc(MAX_TAM_NOME * sizeof(char));
        char *nomeLinha = (char *)malloc(MAX_TAM_NOME * sizeof(char));
        scanf("%d", &codEstacao);
        ScanQuoteString(nomeEstacao);
        nullOrInt(&codLinha);
        nullOrString(nomeLinha); // nome nulo é "-1" pra ficar mais fácil pro meu select
        nullOrInt(&codProxEstacao);
        nullOrInt(&distProxEstacao);
        nullOrInt(&codLinhaIntegra);
        nullOrInt(&codEstacaoIntegra);
        Estacao *estacao = criar_estacao(codEstacao, nomeEstacao, codLinha, nomeLinha, codProxEstacao, distProxEstacao, codLinhaIntegra, codEstacaoIntegra);
        if (_insert(f, estacao))
        {
            destruir_estacao(estacao);
            return EXIT_FAILURE;
        }
        destruir_estacao(estacao);
    }

    return 0;
}


// Façamos o C do CRUD, o celect

int select_all(FILE *f)
{
    // Abrindo arquivo binário, lendo header, verificações etc
    char buffer[TAM_REGISTRO];
    if (f == NULL)
    {
        return EXIT_FAILURE;
    }
    Header *header = ler_header_do_arquivo(f);
    if (header == NULL)
    {
        return EXIT_FAILURE;
    }
    int nroEstacoes = header->nroEstacoes;
    if (nroEstacoes == 0)
    {
        printf("Registro inexistente.\n");
        free(header);
        return 0; // erro tratado localmente, sem necessidade de flag
    }

    fseek(f, TAM_HEADER, SEEK_SET);

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        Estacao *ea = (Estacao *)malloc(sizeof(Estacao));
        escrever_buffer_na_estacao(buffer, ea);
        if (ea->removido == '1')
        {
            destruir_estacao(ea);
            continue;
        }
        utils_imprimir_estacao_ln(ea);

        destruir_estacao(ea);
    }

    free(header);
    return 0;
}

int SELECT(Estacao *estacao_selecao, FILE *f)
{

    char buffer[TAM_REGISTRO];

    Header *header = ler_header_do_arquivo(f);

    if (header == NULL)
    {
        free(header);
        return EXIT_FAILURE;
    }

    int nroEstacoes = header->nroEstacoes;
    if (nroEstacoes == 0)
    {
        printf("Registro inexistente.\n");
        free(header);
        return EXIT_SUCCESS;
    }

    fseek(f, TAM_HEADER, SEEK_SET);

    int achou = 0;

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));
        escrever_buffer_na_estacao(buffer, ea);

        if (ea->removido == '1')
        {
            destruir_estacao(ea);
            continue;
        }

        if (comparar_estacoes(estacao_selecao, ea))
        {
            utils_imprimir_estacao_ln(ea);
            achou = 1; // Encontrou pelo menos um registro nessa consulta, então não mostramos a mensagem de inexistente.
        }

        destruir_estacao(ea);
    }

    if (!achou)
    {
        printf("Registro inexistente.\n");
    }

    free(header);

    return EXIT_SUCCESS;
}

// estacao_valores: valores == 0 -> ignorar e manter;
// valores == -1 -> atualizar para NULO
int UPDATE(Estacao *estacao_busca, Estacao *estacao_valores, FILE *f)
{

    char buffer[TAM_REGISTRO];

    Header *header = ler_header_do_arquivo(f);

    if (header == NULL)
    {
        return EXIT_FAILURE;
    }

    header->status = '0';
    escrever_header_no_arquivo(f, header);

    fseek(f, TAM_HEADER, SEEK_SET);

    Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        
        escrever_buffer_na_estacao(buffer, ea);

        if (ea->removido == '1')
        {
            limpar_estacao(ea);
            continue;
        }

        if (comparar_estacoes(estacao_busca, ea))
        {
            editar_estacao(ea, estacao_valores);
            escrever_estacao_no_buffer(ea, buffer);
            fseek(f, -TAM_REGISTRO, SEEK_CUR);
            escrever_buffer_no_arquivo(f, buffer);
        }

        limpar_estacao(ea);
    }

    header->status = '1';
    
    fseek(f, 0, SEEK_SET);

    escrever_header_no_arquivo(f, header);

    free(header);

    destruir_estacao(ea);

    return EXIT_SUCCESS;
}
