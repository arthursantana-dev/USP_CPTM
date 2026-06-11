#include "CRUD.h"

int crud_select(Estacao *estacao_selecao, FILE *f_dados, FILE *f_ab)
{
    // imprimir_estacao(estacao_selecao);
    char buffer[TAM_REGISTRO];

    Header *header = ler_header_do_arquivo(f_dados);
    header_arvore_b header_b = arvore_b_ler_cabecalho(f_ab);

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

    fseek(f_dados, TAM_HEADER, SEEK_SET);

    int achou = 0;

    Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));

    if(estacao_selecao->codEstacao != -2){
        // printf("entrei 1\n");
        int offset = arvore_b_buscar(f_ab, &header_b, estacao_selecao->codEstacao);
        if(offset == -1){
            printf("Registro inexistente.\n");
        }
        else{
            fseek(f_dados, offset, SEEK_SET);
            fread(buffer, TAM_REGISTRO, 1, f_dados);

            escrever_buffer_na_estacao(buffer, ea);
            // imprimir_estacao(ea);

            if (ea->removido == '1')
            {
                printf("Registro inexistente.\n");
            }
            else
            {
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

    while (fread(buffer, TAM_REGISTRO, 1, f_dados) == 1)
    {
        // Tecnicamente, ea foi lido
        escrever_buffer_na_estacao(buffer, ea);

        if (ea->removido == '1')
        {
            limpar_estacao(ea);
            continue;
        }

        if (comparar_estacoes(estacao_selecao, ea))
        {
            imprimir_estacao(ea);
            achou = 1; // encontrou pelo menos um registro nessa consulta, então não mostramos a mensagem de inexistente.
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

int SELECT_ALL(FILE *f_dados)
{
    // abrindo arquivo binário, lendo header, verificações etc
    char buffer[TAM_REGISTRO];
    if (f_dados== NULL)
    {
        return EXIT_FAILURE;
    }
    Header *header = ler_header_do_arquivo(f_dados);
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

    fseek(f_dados, TAM_HEADER, SEEK_SET);

    Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));

    while (fread(buffer, TAM_REGISTRO, 1, f_dados) == 1)
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

int SELECT(int n, FILE *f_dados, FILE *f_ab)
{
    int err = 0;

    Estacao *estacao_selecao = criar_estacao_para_busca(-2, "", -2, "", -2, -2, -2, -2);

    if (f_dados== NULL)
    {
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++)
    {

        set_valores_estacao_para_busca(estacao_selecao);

        ler_input_para_estacao_de_busca(estacao_selecao);
        err = crud_select(estacao_selecao, f_dados, f_ab);

        limpar_estacao(estacao_selecao);

        if (err)
            break;

        printf("\n");
    }

    destruir_estacao(estacao_selecao);

    return err;
}


