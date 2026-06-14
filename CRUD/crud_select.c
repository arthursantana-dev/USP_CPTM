#include "CRUD.h"

int crud_select(Estacao *estacao_selecao, FILE *f_dados, FILE *f_ab)
{
    // verificações iniciais
    char buffer[TAM_REGISTRO];

    Header *header_dados = ler_header_do_arquivo(f_dados);
    header_btree header_b = btree_ler_cabecalho(f_ab);

    if (header_dados == NULL || header_b.status == '0')
    {
        free(header_dados);
        return EXIT_FAILURE;
    }

    int nroEstacoes = header_dados->nroEstacoes;
    if (nroEstacoes == 0)
    {
        printf("Registro inexistente.\n");
        free(header_dados);
        return EXIT_SUCCESS;
    }

    fseek(f_dados, TAM_HEADER, SEEK_SET);

    int achou = 0;
    Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));

    // prioriza a busca pelo índice caso a chave primária tenha sido informada
    if (estacao_selecao->codEstacao != -2)
    {

        int offset = btree_buscar(f_ab, &header_b, estacao_selecao->codEstacao);
        if (offset == -1)
        {
            printf("Registro inexistente.\n");
        }
        else
        {
            // acessa diretamente o registro no disco sem varredura sequencial
            fseek(f_dados, offset, SEEK_SET);
            fread(buffer, TAM_REGISTRO, 1, f_dados);

            escrever_buffer_na_estacao(buffer, ea);

            if (ea->removido == '1')
            {
                printf("Registro inexistente.\n");
            }
            else
            {
                // confirma se os demais parâmetros de busca não primários batem
                if (comparar_estacoes(estacao_selecao, ea))
                {
                    imprimir_estacao(ea);
                }
                else
                {
                    printf("Registro inexistente.\n");
                }
            }
        }

        destruir_estacao(ea);
        free(header_dados);
        return EXIT_SUCCESS;
    }

    // varredura linear caso a busca seja por campos secundários (ex: nome da estação)
    while (fread(buffer, TAM_REGISTRO, 1, f_dados) == 1)
    {
        // ea foi lido no fread do while
        escrever_buffer_na_estacao(buffer, ea);

        // filtra os registros logicamente removidos
        if (ea->removido == '1')
        {
            limpar_estacao(ea);
            continue;
        }

        // se ea corresponde a busca, imprime
        if (comparar_estacoes(estacao_selecao, ea))
        {
            imprimir_estacao(ea);
            achou = 1;
        }

        limpar_estacao(ea);
    }

    destruir_estacao(ea);

    if (!achou)
    {
        printf("Registro inexistente.\n");
    }

    free(header_dados);

    return EXIT_SUCCESS;
}

int SELECT_ALL(FILE *f_dados)
{
    // abrindo arquivo binário, lendo header, verificações etc
    char buffer[TAM_REGISTRO];
    if (f_dados == NULL)
    {
        return EXIT_FAILURE;
    }
    Header *header_dados = ler_header_do_arquivo(f_dados);
    if (header_dados == NULL)
    {
        mostrar_erro();
        return EXIT_FAILURE;
    }
    int nroEstacoes = header_dados->nroEstacoes;
    if (nroEstacoes == 0)
    {
        printf("Registro inexistente.\n");
        free(header_dados);
        return 0; // erro tratado localmente, sem necessidade de flag
    }

    fseek(f_dados, TAM_HEADER, SEEK_SET);

    Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));

    // iteração no arquivo e impressão de todas as estações
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

    free(header_dados);
    return 0;
}

int SELECT(int n, FILE *f_dados, FILE *f_ab)
{
    int err = 0;

    Estacao *estacao_selecao = criar_estacao_para_busca(-2, "", -2, "", -2, -2, -2, -2);

    if (f_dados == NULL)
    {
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++)
    {

        // formata a struct com -2 para que a verificação descarte os campos não informados
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

// realiza a varredura linear para busca sem utilizar a arvore b
int crud_select_sem_indice(Estacao *estacao_selecao, FILE *f_dados)
{
    char buffer[TAM_REGISTRO];

    Header *header = ler_header_do_arquivo(f_dados);
    if (header == NULL)
    {
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

    // varredura linear completa no arquivo de dados
    while (fread(buffer, TAM_REGISTRO, 1, f_dados) == 1)
    {
        escrever_buffer_na_estacao(buffer, ea);

        // ignora registros logicamente apagados
        if (ea->removido == '1')
        {
            limpar_estacao(ea);
            continue;
        }

        // verifica se os parametros da struct coincidem com o registro
        if (comparar_estacoes(estacao_selecao, ea))
        {
            imprimir_estacao(ea);
            achou = 1;
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

// executa n buscas consecutivas focadas apenas no arquivo de dados
int SELECT_SEM_INDICE(int n, FILE *f_dados)
{
    int err = 0;

    if (f_dados == NULL)
    {
        return EXIT_FAILURE;
    }

    Estacao *estacao_selecao = criar_estacao_para_busca(-2, "", -2, "", -2, -2, -2, -2);

    for (int i = 0; i < n; i++)
    {
        // reseta a struct para ignorar os campos da busca anterior
        set_valores_estacao_para_busca(estacao_selecao);

        ler_input_para_estacao_de_busca(estacao_selecao);
        
        err = crud_select_sem_indice(estacao_selecao, f_dados);

        limpar_estacao(estacao_selecao);

        if (err)
            break;
            
        // mantem o padrao de quebra de linha exigido na saida original
        printf("\n");
    }

    destruir_estacao(estacao_selecao);

    return err;
}