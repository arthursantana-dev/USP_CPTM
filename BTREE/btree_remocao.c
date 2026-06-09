#include "BTREE.h"

/* =========================================================
 * Funcoes Auxiliares de Remocao
 * ========================================================= */

/**
 * @brief Marca uma página no disco como logicamente removida e a empilha no cabeçalho.
 */
static void _remover_pagina(FILE *arquivo, header_arvore_b *cabecalho, int rrn_removido)
{
    no_arvore_b no = arvore_b_ler_no(arquivo, rrn_removido);

    no.removido = '1';
    no.proximo = cabecalho->topo;
    // O resto dos bytes devem permanecer inalterados, conforme especificação.

    arvore_b_escrever_no(arquivo, rrn_removido, &no);

    cabecalho->topo = rrn_removido;
    cabecalho->nro_nos--;
}

/**
 * @brief Desce pela subárvore à direita para encontrar a menor chave (sucessor imediato).
 */
static void _buscar_sucessor(FILE *arquivo, int RRN_atual, int *chave_sucessoressora, int *byte_offset_dado_sucessora)
{
    no_arvore_b no = arvore_b_ler_no(arquivo, RRN_atual);

    // Se for folha, a primeira chave é o sucessor
    if (no.tipo_no == -1)
    {
        *chave_sucessoressora = no.chaves[0];
        *byte_offset_dado_sucessora = no.dados_byte_offsets[0];
        return;
    }

    // Desce sempre pelo ponteiro mais à esquerda
    _buscar_sucessor(arquivo, no.filhos[0], chave_sucessoressora, byte_offset_dado_sucessora);
}

/**
 * @brief Função recursiva principal para a deleção. Retorna true se a página atual sofreu underflow (0 chaves).
 */
static bool _remover_recursivo(FILE *arquivo, header_arvore_b *cabecalho, int RRN_atual, int chave_busca)
{
    if (RRN_atual == -1)
        return false; // Chave não encontrada

    no_arvore_b no = arvore_b_ler_no(arquivo, RRN_atual);

    // i é incrementado até encontrar a posição onde a chave buscada deveria estar (ou o final das chaves)
    int i = 0;
    while (i < no.numero_chaves && chave_busca > no.chaves[i])
    {
        i++;
    }

    bool underflow_filho = false;

    // CASO 1: A chave foi encontrada neste nó
    if (i < no.numero_chaves && chave_busca == no.chaves[i])
    {
        if (no.tipo_no == -1)
        {
            // É folha: Remove fazendo shift para a esquerda
            for (int j = i; j < no.numero_chaves - 1; j++)
            {
                no.chaves[j] = no.chaves[j + 1];
                no.dados_byte_offsets[j] = no.dados_byte_offsets[j + 1];
            }
            no.chaves[no.numero_chaves - 1] = -1;
            no.dados_byte_offsets[no.numero_chaves - 1] = -1;
            no.numero_chaves--;

            arvore_b_escrever_no(arquivo, RRN_atual, &no);
            return (no.numero_chaves == 0); // Ordem 4: Mínimo é 1. Se zerou, deu underflow
        }
        else
        {
            // É nó interno: Troca pelo sucessor imediato (folha)
            int chave_sucessor, byte_offset_dado_sucessor;
            _buscar_sucessor(arquivo, no.filhos[i + 1], &chave_sucessor, &byte_offset_dado_sucessor);

            no.chaves[i] = chave_sucessor;
            no.dados_byte_offsets[i] = byte_offset_dado_sucessor;
            arvore_b_escrever_no(arquivo, RRN_atual, &no);

            // Vai recursivamente remover o sucessor na subárvore
            underflow_filho = _remover_recursivo(arquivo, cabecalho, no.filhos[i + 1], chave_sucessor);
            i++; // Para o tratamento de underflow abaixo saber qual filho deu problema
        }
    }
    else
    {
        // CASO 2: A chave não está neste nó, desce recursivamente
        underflow_filho = _remover_recursivo(arquivo, cabecalho, no.filhos[i], chave_busca);
    }

    // =========================================================================
    // TRATAMENTO DE UNDERFLOW NO FILHO [i]
    // =========================================================================
    if (underflow_filho)
    {
        int rrn_filho = no.filhos[i];
        no_arvore_b filho = arvore_b_ler_no(arquivo, rrn_filho);

        bool tem_irmao_dir = (i < no.numero_chaves);
        bool tem_irmao_esq = (i > 0);

        no_arvore_b irmao_dir, irmao_esq;
        int rrn_irmao_dir = -1, rrn_irmao_esq = -1;

        if (tem_irmao_dir)
        {
            rrn_irmao_dir = no.filhos[i + 1];
            irmao_dir = arvore_b_ler_no(arquivo, rrn_irmao_dir);
        }
        if (tem_irmao_esq)
        {
            rrn_irmao_esq = no.filhos[i - 1];
            irmao_esq = arvore_b_ler_no(arquivo, rrn_irmao_esq);
        }

        // Redistribuição à direita: O irmão direito tem mais de 1 chave, então pode ceder uma chave para o filho
        if (tem_irmao_dir && irmao_dir.numero_chaves > 1)
        {
            filho.chaves[0] = no.chaves[i];
            filho.dados_byte_offsets[0] = no.dados_byte_offsets[i];
            filho.filhos[1] = irmao_dir.filhos[0];
            filho.numero_chaves = 1;

            no.chaves[i] = irmao_dir.chaves[0];
            no.dados_byte_offsets[i] = irmao_dir.dados_byte_offsets[0];

            irmao_dir.filhos[0] = irmao_dir.filhos[1];
            for (int j = 0; j < irmao_dir.numero_chaves - 1; j++)
            {
                irmao_dir.chaves[j] = irmao_dir.chaves[j + 1];
                irmao_dir.dados_byte_offsets[j] = irmao_dir.dados_byte_offsets[j + 1];
                irmao_dir.filhos[j + 1] = irmao_dir.filhos[j + 2];
            }
            irmao_dir.chaves[irmao_dir.numero_chaves - 1] = -1;
            irmao_dir.dados_byte_offsets[irmao_dir.numero_chaves - 1] = -1;
            irmao_dir.filhos[irmao_dir.numero_chaves] = -1;
            irmao_dir.numero_chaves--;

            arvore_b_escrever_no(arquivo, rrn_filho, &filho);
            arvore_b_escrever_no(arquivo, rrn_irmao_dir, &irmao_dir);
            arvore_b_escrever_no(arquivo, RRN_atual, &no);
            return false; // Resolvido
        }

        // Redistribuição à esquerda: O irmão esquerdo tem mais de 1 chave, então pode ceder uma chave para o filho
        if (tem_irmao_esq && irmao_esq.numero_chaves > 1)
        {
            // Filho recebe a chave do pai no índice i-1
            filho.filhos[1] = filho.filhos[0]; // Shift do ponteiro herdado
            filho.chaves[0] = no.chaves[i - 1];
            filho.dados_byte_offsets[0] = no.dados_byte_offsets[i - 1];
            filho.filhos[0] = irmao_esq.filhos[irmao_esq.numero_chaves];
            filho.numero_chaves = 1;

            // Pai recebe a última chave do irmão esquerdo
            no.chaves[i - 1] = irmao_esq.chaves[irmao_esq.numero_chaves - 1];
            no.dados_byte_offsets[i - 1] = irmao_esq.dados_byte_offsets[irmao_esq.numero_chaves - 1];

            // Limpa o espaço cedido no irmão esquerdo
            irmao_esq.chaves[irmao_esq.numero_chaves - 1] = -1;
            irmao_esq.dados_byte_offsets[irmao_esq.numero_chaves - 1] = -1;
            irmao_esq.filhos[irmao_esq.numero_chaves] = -1;
            irmao_esq.numero_chaves--;

            arvore_b_escrever_no(arquivo, rrn_filho, &filho);
            arvore_b_escrever_no(arquivo, rrn_irmao_esq, &irmao_esq);
            arvore_b_escrever_no(arquivo, RRN_atual, &no);
            return false; // Resolvido
        }

        // Concatenação à esquerda: O irmão esquerdo tem apenas 1 chave, então é necessário fundir o filho com o irmão esquerdo e puxar a chave do pai para esse meio-fio
        if (tem_irmao_esq)
        {
            // Irmão Esquerdo absorve a chave do Pai e todo o conteúdo do Filho (que estava vazio de chaves)
            irmao_esq.chaves[irmao_esq.numero_chaves] = no.chaves[i - 1];
            irmao_esq.dados_byte_offsets[irmao_esq.numero_chaves] = no.dados_byte_offsets[i - 1];
            irmao_esq.filhos[irmao_esq.numero_chaves + 1] = filho.filhos[0];
            irmao_esq.numero_chaves++;

            // A página do Filho (à direita da junção) é destruída
            _remover_pagina(arquivo, cabecalho, rrn_filho);

            // Shift no Pai para remover a chave [i-1] que desceu e o ponteiro [i] que apontava para o Filho morto
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

            arvore_b_escrever_no(arquivo, rrn_irmao_esq, &irmao_esq);
            arvore_b_escrever_no(arquivo, RRN_atual, &no);
            return (no.numero_chaves == 0); // Repassa o sob-aviso se o pai zerou
        }

        // Concatenação à direita: O irmão direito tem apenas 1 chave, então é necessário fundir o filho com o irmão direito e puxar a chave do pai para esse meio-fio - filho esquerdo
        if (tem_irmao_dir)
        {
            // O Filho (atual, sem chaves) absorve a chave do Pai e todo o conteúdo do Irmão Direito
            filho.chaves[0] = no.chaves[i];
            filho.dados_byte_offsets[0] = no.dados_byte_offsets[i];
            filho.filhos[1] = irmao_dir.filhos[0];

            // Traz as chaves e ponteiros do irmão direito para o filho
            for (int j = 0; j < irmao_dir.numero_chaves; j++)
            {
                filho.chaves[1 + j] = irmao_dir.chaves[j];
                filho.dados_byte_offsets[1 + j] = irmao_dir.dados_byte_offsets[j];
                filho.filhos[2 + j] = irmao_dir.filhos[j + 1];
            }
            filho.numero_chaves = 1 + irmao_dir.numero_chaves;

            // A página do Irmão Direito é destruída
            _remover_pagina(arquivo, cabecalho, rrn_irmao_dir);

            // Shift no Pai para remover a chave [i] que desceu e o ponteiro [i+1]
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

            arvore_b_escrever_no(arquivo, rrn_filho, &filho);
            arvore_b_escrever_no(arquivo, RRN_atual, &no);
            return (no.numero_chaves == 0); // Repassa o sob-aviso se o pai zerou
        }
    }

    return false;
}

/**
 * @brief Dispara e gerencia a deleção lógica da raiz.
 */
void arvore_b_remover(FILE *arquivo, header_arvore_b *cabecalho, int chave_busca)
{
    if (arquivo == NULL || cabecalho == NULL || cabecalho->no_raiz == -1)
        return;

    bool underflow_raiz = _remover_recursivo(arquivo, cabecalho, cabecalho->no_raiz, chave_busca);

    // Se a raiz sofreu underflow e zerou suas chaves
    if (underflow_raiz)
    {
        no_arvore_b raiz = arvore_b_ler_no(arquivo, cabecalho->no_raiz);

        // Se ela tiver um descendente, esse descendente vira a nova raiz absoluta
        if (raiz.tipo_no != -1)
        {
            int nova_raiz_rrn = raiz.filhos[0];
            _remover_pagina(arquivo, cabecalho, cabecalho->no_raiz);
            cabecalho->no_raiz = nova_raiz_rrn;

            no_arvore_b nova_raiz = arvore_b_ler_no(arquivo, nova_raiz_rrn);
            nova_raiz.tipo_no = 0; // A nova raiz é promovida a raiz
            arvore_b_escrever_no(arquivo, nova_raiz_rrn, &nova_raiz);
        }
        else
        {
            // Se a raiz folha secou, a árvore está 100% vazia
            _remover_pagina(arquivo, cabecalho, cabecalho->no_raiz);
            cabecalho->no_raiz = -1;
        }
    }

    // Assim como na inserção, é crucial gravar o cabeçalho no disco ao fim
    arvore_b_atualizar_cabecalho(arquivo, cabecalho);
}
