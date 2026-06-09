#include "BTREE.h"

/*Inserção*/

/**
 * @brief Obtém um RRN livre, seja reaproveitando o lixo ou expandindo o arquivo.
 */
int _obter_rrn_livre(FILE *arquivo, header_arvore_b *cabecalho)
{
    int rrn;

    // Se há registros removidos na pilha, reaproveita o topo
    if (cabecalho->topo != -1)
    {
        rrn = cabecalho->topo;
        no_arvore_b no_removido = arvore_b_ler_no(arquivo, rrn);
        cabecalho->topo = no_removido.proximo; // Atualiza o topo para o próximo removido
    }
    else
    {
        // Se não há removidos, pega o RRN do fim do arquivo e incrementa
        rrn = cabecalho->prox_RRN;
        cabecalho->prox_RRN++;
    }

    cabecalho->nro_nos++; // Incrementa o número de nós para a nova raiz

    return rrn;
}

/**
 * @brief Função interna que desce a árvore até a folha e trata os splits na subida.
 */
retorno_insercao_t _inserir_recursivo(FILE *arquivo, header_arvore_b *cabecalho, int RRN_atual, int chave, int byte_offset_dado)
{
    retorno_insercao_t ret = {false, -1, -1, -1};

    // printf("Descendo para RRN: %d\n", RRN_atual);

    // Caso Base: Chegou em um ponteiro nulo, a chave deve ser inserida na folha pai
    if (RRN_atual == -1)
    {
        ret.houve_split = true;
        ret.chave_promovida = chave;
        ret.RRN_dado_promovido = byte_offset_dado;
        ret.RRN_filho_direito = -1;
        return ret;
    }

    no_arvore_b no = arvore_b_ler_no(arquivo, RRN_atual);

    // Busca a posição correta no nó
    int i = 0;
    while (i < no.numero_chaves && chave > no.chaves[i])
    {
        i++;
    }

    // Se a chave já existir, a inserção é ignorada
    if (i < no.numero_chaves && chave == no.chaves[i])
    {
        return ret;
    }

    // Desce para o filho apropriado
    ret = _inserir_recursivo(arquivo, cabecalho, no.filhos[i], chave, byte_offset_dado);

    /*
        Volta da recursão
    */

    // Se o filho não splitou, a estrutura de baixo já resolveu a inserção
    if (!ret.houve_split)
    {
        return ret;
    }

    // --- Filho splitou, tenta alocar a chave promovida no NÓ ATUAL ---

    // Caso 1: Nó atual tem espaço
    if (no.numero_chaves < MAX_CHAVES)
    {
        // Faz o "shift" para abrir espaço para a nova chave
        for (int j = no.numero_chaves; j > i; j--)
        {
            no.chaves[j] = no.chaves[j - 1];
            no.dados_byte_offsets[j] = no.dados_byte_offsets[j - 1];
            no.filhos[j + 1] = no.filhos[j];
        }

        no.chaves[i] = ret.chave_promovida;
        no.dados_byte_offsets[i] = ret.RRN_dado_promovido;
        no.filhos[i + 1] = ret.RRN_filho_direito;
        no.numero_chaves++;

        arvore_b_escrever_no(arquivo, RRN_atual, &no);

        ret.houve_split = false; // O split foi absorvido neste nó
        return ret;
    }

    // --- Caso 2: Nó atual não tem espaço (SPLIT) ---
    // A redistribuição não deve ser implementada na inserção, forçando um particionamento direto.

    // Nó pai: vindo de um filho que houve promoção.

    // Arrays temporários para organizar as chaves em ordem
    int temp_chaves[MAX_CHAVES + 1];
    int temp_dados_byte_offsets[MAX_CHAVES + 1];
    int temp_filhos[MAX_CHAVES + 2];

    for (int j = 0, idx = 0; j < MAX_CHAVES; j++, idx++)
    {
        if (idx == i)
            idx++; // Pula o índice onde a chave promovida vai entrar
        temp_chaves[idx] = no.chaves[j];
        temp_dados_byte_offsets[idx] = no.dados_byte_offsets[j];
    }
    temp_chaves[i] = ret.chave_promovida;
    temp_dados_byte_offsets[i] = ret.RRN_dado_promovido;

    for (int j = 0, idx = 0; j < MAX_CHAVES + 1; j++, idx++)
    {
        if (idx == i + 1)
            idx++; // Pula o ponteiro do novo filho
        temp_filhos[idx] = no.filhos[j];
    }
    temp_filhos[i + 1] = ret.RRN_filho_direito;

    // Reseta o nó original (Nó da Esquerda)
    for (int j = 0; j < MAX_CHAVES; j++)
    {
        no.chaves[j] = -1;
        no.dados_byte_offsets[j] = -1;
        no.filhos[j] = -1;
    }
    no.filhos[MAX_CHAVES] = -1;

    // A distribuição deve ser a mais uniforme possível, com o nó esquerdo contendo 2 chaves e o direito 1 chave.
    no.numero_chaves = 2;
    no.chaves[0] = temp_chaves[0];
    no.dados_byte_offsets[0] = temp_dados_byte_offsets[0];
    no.chaves[1] = temp_chaves[1];
    no.dados_byte_offsets[1] = temp_dados_byte_offsets[1];
    no.filhos[0] = temp_filhos[0];
    no.filhos[1] = temp_filhos[1];
    no.filhos[2] = temp_filhos[2];

    // O tipo do nó original é ajustado: se ele era raiz (0), passa a ser intermediário (1). Folhas continuam -1.
    if (no.tipo_no == 0)
        no.tipo_no = 1;

    // Cria o Novo Nó (Nó da Direita)
    int rrn_novo_no = _obter_rrn_livre(arquivo, cabecalho);
    no_arvore_b novo_no = arvore_b_criar_no_vazio();

    novo_no.tipo_no = no.tipo_no; // Copia a tipagem do irmão recém rebaixado
    novo_no.numero_chaves = 1;
    novo_no.chaves[0] = temp_chaves[3];
    novo_no.dados_byte_offsets[0] = temp_dados_byte_offsets[3];
    novo_no.filhos[0] = temp_filhos[3];
    novo_no.filhos[1] = temp_filhos[4];

    // A chave promovida passa a ser a primeira do nó resultante do particionamento (índice 2 do array temporário).
    ret.houve_split = true;
    ret.chave_promovida = temp_chaves[2];
    ret.RRN_dado_promovido = temp_dados_byte_offsets[2];
    ret.RRN_filho_direito = rrn_novo_no;

    // Salva ambos no disco
    arvore_b_escrever_no(arquivo, RRN_atual, &no);
    arvore_b_escrever_no(arquivo, rrn_novo_no, &novo_no);

    return ret;
}

void arvore_b_inserir(FILE *arquivo, header_arvore_b *cabecalho, int chave, int byte_offset_dado)
{

    // Caso base: Árvore vazia, cria o nó raiz
    if (cabecalho->no_raiz == -1)
    {
        no_arvore_b raiz = arvore_b_criar_no_vazio();
        raiz.tipo_no = -1; // Nó folha
        raiz.numero_chaves = 1;
        raiz.chaves[0] = chave;
        raiz.dados_byte_offsets[0] = byte_offset_dado;

        int rrn_raiz = _obter_rrn_livre(arquivo, cabecalho);
        arvore_b_escrever_no(arquivo, rrn_raiz, &raiz);

        // printf("RRN da raiz: %d\n", rrn_raiz);

        cabecalho->no_raiz = rrn_raiz; // Atualiza o RRN da raiz no cabeçalho
        cabecalho->nro_nos = 1;        // Atualiza o número de nós

        // printf("numero de nos: %d\n", cabecalho->nro_nos);

        arvore_b_atualizar_cabecalho(arquivo, cabecalho); // Atualiza o cabeçalho no disco após a modificação

        return;
    }

    // Chama a função recursiva de inserção a partir da raiz
    retorno_insercao_t resultado = _inserir_recursivo(arquivo, cabecalho, cabecalho->no_raiz, chave, byte_offset_dado);

    // Se houve split na raiz, precisamos criar um novo nó raiz
    if (resultado.houve_split)
    {
        no_arvore_b nova_raiz = arvore_b_criar_no_vazio();
        nova_raiz.tipo_no = 0; // Nó raiz
        nova_raiz.numero_chaves = 1;
        nova_raiz.chaves[0] = resultado.chave_promovida;
        nova_raiz.dados_byte_offsets[0] = resultado.RRN_dado_promovido;
        nova_raiz.filhos[0] = cabecalho->no_raiz;          // Filho esquerdo é a antiga raiz
        nova_raiz.filhos[1] = resultado.RRN_filho_direito; // Filho direito é o novo nó criado

        int rrn_nova_raiz = _obter_rrn_livre(arquivo, cabecalho);
        arvore_b_escrever_no(arquivo, rrn_nova_raiz, &nova_raiz);

        cabecalho->no_raiz = rrn_nova_raiz; // Atualiza o RRN da nova raiz no cabeçalho
    }

    arvore_b_atualizar_cabecalho(arquivo, cabecalho); // Atualiza o cabeçalho no disco após a modificação

    // printf("numero de nos: %d\n", cabecalho->nro_nos);
}
