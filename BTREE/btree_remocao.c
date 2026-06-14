#include "BTREE.h"

// funcoes auxiliares de remocao

/**
 * @brief Marca uma página no disco como logicamente removida e a empilha no cabeçalho.
 */
static void _remover_pagina(FILE *arquivo, header_btree *cabecalho, int rrn_removido)
{
    no_btree no = btree_ler_no(arquivo, rrn_removido);

    no.removido = '1';
    no.proximo = cabecalho->topo;
    // O resto dos bytes devem permanecer inalterados, conforme especificação.

    btree_escrever_no(arquivo, rrn_removido, &no);

    cabecalho->topo = rrn_removido;
    cabecalho->nro_nos--;
}

/**
 * @brief Desce pela subárvore à direita para encontrar a menor chave (sucessor imediato).
 */
static void _buscar_sucessor(FILE *arquivo, int RRN_atual, int *chave_sucessoressora, int *byte_offset_dado_sucessora)
{
    no_btree no = btree_ler_no(arquivo, RRN_atual);

    // se for folha, a primeira chave é o sucessor
    if (no.tipo_no == -1)
    {
        *chave_sucessoressora = no.chaves[0];
        *byte_offset_dado_sucessora = no.dados_byte_offsets[0];
        return;
    }

    // desce sempre pelo ponteiro mais à esquerda
    _buscar_sucessor(arquivo, no.filhos[0], chave_sucessoressora, byte_offset_dado_sucessora);
}

/**
 * @brief Função recursiva principal para a deleção. Retorna true se a página atual sofreu underflow (0 chaves).
 */
static bool _remover_recursivo(FILE *arquivo, header_btree *cabecalho, int RRN_atual, int chave_busca)
{
    if (RRN_atual == -1)
        return false; // chave não encontrada

    no_btree no = btree_ler_no(arquivo, RRN_atual);

    // i é incrementado até encontrar a posição onde a chave buscada deveria estar (ou o final das chaves)
    int i = 0;
    while (i < no.numero_chaves && chave_busca > no.chaves[i])
    {
        i++;
    }

    bool underflow_filho = false;

    // caso 1: A chave foi encontrada neste nó
    if (i < no.numero_chaves && chave_busca == no.chaves[i])
    {
        if (no.tipo_no == -1)
        {
            // é folha: Remove fazendo shift para a esquerda
            for (int j = i; j < no.numero_chaves - 1; j++)
            {
                no.chaves[j] = no.chaves[j + 1];
                no.dados_byte_offsets[j] = no.dados_byte_offsets[j + 1];
            }
            no.chaves[no.numero_chaves - 1] = -1;
            no.dados_byte_offsets[no.numero_chaves - 1] = -1;
            no.numero_chaves--;

            btree_escrever_no(arquivo, RRN_atual, &no);
            return (no.numero_chaves == 0); // Ordem 4: Mínimo é 1. Se zerou, deu underflow
        }
        else
        {
            // é nó interno: Troca pelo sucessor imediato (folha)
            int chave_sucessor, byte_offset_dado_sucessor;
            _buscar_sucessor(arquivo, no.filhos[i + 1], &chave_sucessor, &byte_offset_dado_sucessor);

            no.chaves[i] = chave_sucessor;
            no.dados_byte_offsets[i] = byte_offset_dado_sucessor;
            btree_escrever_no(arquivo, RRN_atual, &no);

            // vai recursivamente remover o sucessor na subárvore
            underflow_filho = _remover_recursivo(arquivo, cabecalho, no.filhos[i + 1], chave_sucessor);
            i++; // para o tratamento de underflow abaixo saber qual filho deu problema
        }
    }
    else
    {
        // caso 2: A chave não está neste nó, desce recursivamente
        underflow_filho = _remover_recursivo(arquivo, cabecalho, no.filhos[i], chave_busca);
    }

    // tratamento de underflow no filho
    if (underflow_filho)
    {
        int rrn_filho = no.filhos[i];
        no_btree filho = btree_ler_no(arquivo, rrn_filho);

        bool tem_irmao_direito = (i < no.numero_chaves);
        bool tem_irmao_esquerdo = (i > 0);

        no_btree irmao_direito, irmao_esquerdo;
        int rrn_irmao_direito = -1, rrn_irmao_esquerdo = -1;

        if (tem_irmao_direito)
        {
            rrn_irmao_direito = no.filhos[i + 1];
            irmao_direito = btree_ler_no(arquivo, rrn_irmao_direito);
        }
        if (tem_irmao_esquerdo)
        {
            rrn_irmao_esquerdo = no.filhos[i - 1];
            irmao_esquerdo = btree_ler_no(arquivo, rrn_irmao_esquerdo);
        }

        // redistribuição à direita: O irmão direito tem mais de 1 chave, então pode ceder uma chave para o filho
        if (tem_irmao_direito && irmao_direito.numero_chaves > 1)
        {
            filho.chaves[0] = no.chaves[i];
            filho.dados_byte_offsets[0] = no.dados_byte_offsets[i];
            filho.filhos[1] = irmao_direito.filhos[0];
            filho.numero_chaves = 1;

            no.chaves[i] = irmao_direito.chaves[0];
            no.dados_byte_offsets[i] = irmao_direito.dados_byte_offsets[0];

            irmao_direito.filhos[0] = irmao_direito.filhos[1];
            for (int j = 0; j < irmao_direito.numero_chaves - 1; j++)
            {
                irmao_direito.chaves[j] = irmao_direito.chaves[j + 1];
                irmao_direito.dados_byte_offsets[j] = irmao_direito.dados_byte_offsets[j + 1];
                irmao_direito.filhos[j + 1] = irmao_direito.filhos[j + 2];
            }
            irmao_direito.chaves[irmao_direito.numero_chaves - 1] = -1;
            irmao_direito.dados_byte_offsets[irmao_direito.numero_chaves - 1] = -1;
            irmao_direito.filhos[irmao_direito.numero_chaves] = -1;
            irmao_direito.numero_chaves--;

            btree_escrever_no(arquivo, rrn_filho, &filho);
            btree_escrever_no(arquivo, rrn_irmao_direito, &irmao_direito);
            btree_escrever_no(arquivo, RRN_atual, &no);
            return false; // resolvido
        }

        // redistribuição à esquerda: O irmão esquerdo tem mais de 1 chave, então pode ceder uma chave para o filho
        if (tem_irmao_esquerdo && irmao_esquerdo.numero_chaves > 1)
        {
            // filho recebe a chave do pai no índice i-1
            filho.filhos[1] = filho.filhos[0]; // shift do ponteiro herdado
            filho.chaves[0] = no.chaves[i - 1];
            filho.dados_byte_offsets[0] = no.dados_byte_offsets[i - 1];
            filho.filhos[0] = irmao_esquerdo.filhos[irmao_esquerdo.numero_chaves];
            filho.numero_chaves = 1;

            // pai recebe a última chave do irmão esquerdo
            no.chaves[i - 1] = irmao_esquerdo.chaves[irmao_esquerdo.numero_chaves - 1];
            no.dados_byte_offsets[i - 1] = irmao_esquerdo.dados_byte_offsets[irmao_esquerdo.numero_chaves - 1];

            // limpa o espaço cedido no irmão esquerdo
            irmao_esquerdo.chaves[irmao_esquerdo.numero_chaves - 1] = -1;
            irmao_esquerdo.dados_byte_offsets[irmao_esquerdo.numero_chaves - 1] = -1;
            irmao_esquerdo.filhos[irmao_esquerdo.numero_chaves] = -1;
            irmao_esquerdo.numero_chaves--;

            btree_escrever_no(arquivo, rrn_filho, &filho);
            btree_escrever_no(arquivo, rrn_irmao_esquerdo, &irmao_esquerdo);
            btree_escrever_no(arquivo, RRN_atual, &no);
            return false; // resolvido
        }

        // concatenação à esquerda: O irmão esquerdo tem apenas 1 chave, então é necessário fundir o filho com o irmão esquerdo e puxar a chave do pai para esse meio-fio
        if (tem_irmao_esquerdo)
        {
            // irmão Esquerdo absorve a chave do Pai e todo o conteúdo do Filho (que estava vazio de chaves)
            irmao_esquerdo.chaves[irmao_esquerdo.numero_chaves] = no.chaves[i - 1];
            irmao_esquerdo.dados_byte_offsets[irmao_esquerdo.numero_chaves] = no.dados_byte_offsets[i - 1];
            irmao_esquerdo.filhos[irmao_esquerdo.numero_chaves + 1] = filho.filhos[0];
            irmao_esquerdo.numero_chaves++;

            // a página do Filho (à direita da junção) é destruída
            _remover_pagina(arquivo, cabecalho, rrn_filho);

            // shift no Pai para remover a chave [i-1] que desceu e o ponteiro [i] que apontava para o Filho morto
            for (int j = i - 1; j < no.numero_chaves - 1; j++)
            {
                no.chaves[j] = no.chaves[j + 1];
                no.dados_byte_offsets[j] = no.dados_byte_offsets[j + 1];
                no.filhos[j + 1] = no.filhos[j + 2];
            }
            no.chaves[no.numero_chaves - 1] = -1;
            no.dados_byte_offsets[no.numero_chaves - 1] = -1;
            no.filhos[no.numero_chaves] = -1;
            no.numero_chaves--;

            btree_escrever_no(arquivo, rrn_irmao_esquerdo, &irmao_esquerdo);
            btree_escrever_no(arquivo, RRN_atual, &no);
            return (no.numero_chaves == 0); // repassa o sob-aviso se o pai zerou
        }

        // concatenação à direita: O irmão direito tem apenas 1 chave, então é necessário fundir o filho com o irmão direito e puxar a chave do pai para esse meio-fio - filho esquerdo
        if (tem_irmao_direito)
        {
            // O Filho (atual, sem chaves) absorve a chave do Pai e todo o conteúdo do Irmão Direito
            filho.chaves[0] = no.chaves[i];
            filho.dados_byte_offsets[0] = no.dados_byte_offsets[i];
            filho.filhos[1] = irmao_direito.filhos[0];

            // Traz as chaves e ponteiros do irmão direito para o filho
            for (int j = 0; j < irmao_direito.numero_chaves; j++)
            {
                filho.chaves[1 + j] = irmao_direito.chaves[j];
                filho.dados_byte_offsets[1 + j] = irmao_direito.dados_byte_offsets[j];
                filho.filhos[2 + j] = irmao_direito.filhos[j + 1];
            }
            filho.numero_chaves = 1 + irmao_direito.numero_chaves;

            // a página do Irmão Direito é destruída
            _remover_pagina(arquivo, cabecalho, rrn_irmao_direito);

            // shift no Pai para remover a chave [i] que desceu e o ponteiro [i+1]
            for (int j = i; j < no.numero_chaves - 1; j++)
            {
                no.chaves[j] = no.chaves[j + 1];
                no.dados_byte_offsets[j] = no.dados_byte_offsets[j + 1];
                no.filhos[j + 1] = no.filhos[j + 2];
            }
            no.chaves[no.numero_chaves - 1] = -1;
            no.dados_byte_offsets[no.numero_chaves - 1] = -1;
            no.filhos[no.numero_chaves] = -1;
            no.numero_chaves--;

            btree_escrever_no(arquivo, rrn_filho, &filho);
            btree_escrever_no(arquivo, RRN_atual, &no);
            return (no.numero_chaves == 0); // repassa o sob-aviso se o pai zerou
        }
    }

    return false;
}

/**
 * @brief Dispara e gerencia a deleção lógica da raiz.
 */
void btree_remover(FILE *arquivo, header_btree *cabecalho, int chave_busca)
{
    if (arquivo == NULL || cabecalho == NULL || cabecalho->no_raiz == -1)
        return;

    bool underflow_raiz = _remover_recursivo(arquivo, cabecalho, cabecalho->no_raiz, chave_busca);

    // se a raiz sofreu underflow e zerou suas chaves
    if (underflow_raiz)
    {
        no_btree raiz = btree_ler_no(arquivo, cabecalho->no_raiz);

        // se ela tiver um descendente, esse descendente vira a nova raiz absoluta
        if (raiz.tipo_no != -1)
        {
            int nova_raiz_rrn = raiz.filhos[0];
            _remover_pagina(arquivo, cabecalho, cabecalho->no_raiz);
            cabecalho->no_raiz = nova_raiz_rrn;

            no_btree nova_raiz = btree_ler_no(arquivo, nova_raiz_rrn);

            if (nova_raiz.tipo_no == 1)
                nova_raiz.tipo_no = 0;

            btree_escrever_no(arquivo, nova_raiz_rrn, &nova_raiz);
        }
        else
        {
            // se a raiz folha secou, a árvore está 100% vazia
            _remover_pagina(arquivo, cabecalho, cabecalho->no_raiz);
            cabecalho->no_raiz = -1;
        }
    }

    btree_atualizar_cabecalho(arquivo, cabecalho);
}
