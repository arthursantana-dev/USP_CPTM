#include "CRUD.h"

// estacao_valores: valores == -2 -> ignorar e manter;
// valores == -1 -> atualizar para NULO
int crud_update(Estacao *estacao_busca, Estacao *estacao_valores, FILE *f_dados)
{

    char buffer[TAM_REGISTRO];

    Header *header = ler_header_do_arquivo(f_dados);

    if (header == NULL)
    {
        mostrar_erro();
        return EXIT_FAILURE;
    }

    header->status = '0';
    escrever_header_no_arquivo(f_dados, header);
    
    // indo para o primeiro registro
    fseek(f_dados, TAM_HEADER, SEEK_SET);

    Estacao *ea = (Estacao *)calloc(1, sizeof(Estacao));

    while (fread(buffer, TAM_REGISTRO, 1, f_dados) == 1)
    {
        // armazena a estação do arquivo na estação A
        escrever_buffer_na_estacao(buffer, ea);

        // se logicamente removido, pula para o próximo registro
        if (ea->removido == '1')
        {
            limpar_estacao(ea);
            continue;
        }

        // se a estação A for correspondente à busca, atualiza os valores
        if (comparar_estacoes(estacao_busca, ea))
        {
            copiar_estacao(ea, estacao_valores);
            escrever_estacao_no_buffer(ea, buffer);
            fseek(f_dados, -TAM_REGISTRO, SEEK_CUR);
            escrever_buffer_no_arquivo(f_dados, buffer);
        }

        limpar_estacao(ea);
    }

    header->status = '1';

    fseek(f_dados, 0, SEEK_SET);

    escrever_header_no_arquivo(f_dados, header);

    free(header);

    destruir_estacao(ea);

    return EXIT_SUCCESS;
}

int UPDATE(int n, FILE *f_dados)
{
    int err = 0;

    if (f_dados == NULL)
    {
        mostrar_erro();
        return EXIT_FAILURE;
    }

    // criação de duas estações 
    Estacao *estacao_busca = criar_estacao_para_busca(-2, "", -2, "", -2, -2, -2, -2);

    Estacao *estacao_valores = criar_estacao_para_busca(-2, "", -2, "", -2, -2, -2, -2);

    for (int i = 0; i < n; i++)
    {
        // estaçao_busca serve pra fazer busca
        // estacao_valores serve pra dizer quais campos atualizar e com que valores
        // mesma lógica de valores de campo
        set_valores_estacao_para_busca(estacao_busca);
        set_valores_estacao_para_busca(estacao_valores);

        ler_input_para_estacao_de_busca(estacao_busca);
        ler_input_para_estacao_de_busca(estacao_valores);

        err = crud_update(estacao_busca, estacao_valores, f_dados);

        limpar_estacao(estacao_busca);
        limpar_estacao(estacao_valores);

        if (err)
            break;

        if(i < n-1) printf("\n");
    }

    destruir_estacao(estacao_busca);
    destruir_estacao(estacao_valores);

    return err;

}

