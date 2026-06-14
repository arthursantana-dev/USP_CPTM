#include "CRUD.h"

int crud_select(Estacao *estacao_selecao, FILE *f, FILE *fab)
{
    // verificações iniciais
    char buffer[TAM_REGISTRO];

    Header *header = ler_header_do_arquivo(f);
    header_arvore_b header_b = arvore_b_ler_cabecalho(fab);

    if (header == NULL || header_b.status == '0')
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

    Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));

    // fluxo separado para codEstação (se achar, para a busca)
    if(estacao_selecao->codEstacao != -2){
        int offset = arvore_b_buscar(fab, &header_b, estacao_selecao->codEstacao);
        if(offset == -1){
            printf("Registro inexistente.\n");
        }
        else{
            fseek(f, offset, SEEK_SET);
            fread(buffer, TAM_REGISTRO, 1, f);

            escrever_buffer_na_estacao(buffer, ea);
            if (ea->removido == '1')
            {
                printf("Registro inexistente.\n");
            }
            else
            {
                // verificando se os outros campos batem
                if(comparar_estacoes(estacao_selecao, ea)){
                    imprimir_estacao(ea);
                }
                else{
                    printf("Registro inexistente.\n");
                }
            }
        }

        destruir_estacao(ea);
        free(header);
        return EXIT_SUCCESS;
    }

    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        // ea foi lido no fread do while
        escrever_buffer_na_estacao(buffer, ea);

        if (ea->removido == '1')
        {
            limpar_estacao(ea);
            continue;
        }

        // se ea corresponde a busca, imprime
        if (comparar_estacoes(estacao_selecao, ea))
        {
            imprimir_estacao(ea);
            achou = 1; // Encontrou pelo menos um registro nessa consulta, então não mostramos a mensagem de inexistente.
        }

        limpar_estacao(ea);
    }

    destruir_estacao(ea);

    if (!achou)
    {
        printf("Registro inexistente.\n");
    }

    free(header);

    return EXIT_SUCCESS;
}

int SELECT_ALL(FILE *f)
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
        mostrar_erro();
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

    Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));

    // iteração no arquivo e impressão de todas as estações
    while (fread(buffer, TAM_REGISTRO, 1, f) == 1)
    {
        escrever_buffer_na_estacao(buffer, ea);
        if (ea->removido == '1')
        {
            limpar_estacao(ea);
            continue;
        }
        imprimir_estacao(ea);

        limpar_estacao(ea);
    }

    destruir_estacao(ea);

    free(header);
    return 0;
}

int SELECT(int n, FILE *f, FILE *fab)
{
    int err = 0;

    Estacao *estacao_selecao = criar_estacao_para_busca(-2, "", -2, "", -2, -2, -2, -2);

    if (f == NULL)
    {
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++)
    {

        set_valores_estacao_para_busca(estacao_selecao);

        ler_input_para_estacao_de_busca(estacao_selecao);
        err = crud_select(estacao_selecao, f, fab);

        limpar_estacao(estacao_selecao);

        if (err)
            break;

        printf("\n");
    }

    destruir_estacao(estacao_selecao);

    return err;
}


