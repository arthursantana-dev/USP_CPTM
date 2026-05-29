#include "CRUD.h"
#include "../ArvoreB/ArvoreB.h"

int CREATE(char *nome_arquivo_csv, char *nome_arquivo_binario)
{

    char *buffer = criar_buffer();

    FILE *csv = fopen(nome_arquivo_csv, "r");
    if (csv == NULL)
    {
        return EXIT_FAILURE;
    }

    char cabecalho[256];
    fgets(cabecalho, sizeof(cabecalho), csv);

    FILE *bin = fopen(nome_arquivo_binario, "wb");
    if (bin == NULL)
    {
        free(buffer);
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


int CREATE_INDEX(char *nome_arquivo_binario, char* nome_arquivo_arvore_b){
    FILE* bin = fopen(nome_arquivo_binario, "rb");

    if(bin == NULL){
        return EXIT_FAILURE;
    }

    FILE* arquivo_arvore_b = arvore_b_abrir_escrita(nome_arquivo_arvore_b);

    if(arquivo_arvore_b == NULL){
        fclose(bin);
        return EXIT_FAILURE;
    }

    header_arvore_b header_b = arvore_b_ler_cabecalho(arquivo_arvore_b);

    header_b.status = '0';
    arvore_b_atualizar_cabecalho(arquivo_arvore_b, &header_b);

    char* buffer = criar_buffer();

    Estacao* estacao = criar_estacao_para_busca(0, "", 0, "", 0, 0, 0, 0);

    int RRN = 0;

    int removido = 0;

    fseek(bin, TAM_HEADER, SEEK_SET);

    while (fread(buffer, TAM_REGISTRO, 1, bin) == 1){

        removido = escrever_buffer_na_estacao(buffer, estacao);

        if(removido == 1){
            limpar_estacao(estacao);
            removido = 0;
            RRN++;
            continue;
        }

        imprimir_estacao(estacao);

        int byteoffset = TAM_HEADER + (RRN * TAM_REGISTRO);

        arvore_b_inserir(arquivo_arvore_b, &header_b, estacao->codEstacao, byteoffset);

        limpar_estacao(estacao);

        RRN++;
    }

    header_b.status = '1';
    arvore_b_atualizar_cabecalho(arquivo_arvore_b, &header_b);

    arvore_b_fechar(arquivo_arvore_b, &header_b);

    fclose(bin);

    destruir_estacao(estacao);

    free(buffer);

    BinarioNaTela(nome_arquivo_arvore_b);

    return EXIT_SUCCESS;
}