#include "ArvoreB.h"
#include <stdlib.h>

/*Cabeçalho*/

header_arvore_b arvore_b_ler_cabecalho(FILE *arquivo)
{
    header_arvore_b cabecalho;

    fseek(arquivo, 0, SEEK_SET);
    fread(&cabecalho.status, sizeof(char), 1, arquivo);
    fread(&cabecalho.no_raiz, sizeof(int), 1, arquivo);
    fread(&cabecalho.topo, sizeof(int), 1, arquivo);
    fread(&cabecalho.prox_RRN, sizeof(int), 1, arquivo);
    fread(&cabecalho.nro_nos, sizeof(int), 1, arquivo);

    return cabecalho;
}

// Estrutura de retorno para guiar a subida recursiva do split
// @param houve_split Indica se ocorreu um split no nível inferior
// @param chave_promovida A chave que deve ser promovida para o nível superior
// @param RRN_dado_promovido O PR associado à chave promovida (relevante para folhas)
// @param RRN_filho_direito O RRN do novo nó criado à direita após o split
typedef struct
{
    bool houve_split;
    int chave_promovida;
    int RRN_dado_promovido;
    int RRN_filho_direito;
} retorno_insercao_t;

no_arvore_b arvore_b_criar_no_vazio(void)
{
    no_arvore_b no;
    no.removido = '0';
    no.proximo = -1;
    no.tipo_no = -1;
    no.numero_chaves = 0;

    for (int i = 0; i < MAX_CHAVES; i++)
    {
        no.chaves[i] = -1;
        no.dados_byte_offsets[i] = -1;
    }
    for (int i = 0; i < ORDEM; i++)
    {
        no.filhos[i] = -1;
    }
    return no;
}

FILE *arvore_b_abrir_escrita(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "wb+");
    if (arquivo == NULL)
    {
        perror("Erro ao criar o arquivo de índice");
        exit(EXIT_FAILURE);
    }

    // Inicializa o cabeçalho com valores padrão
    header_arvore_b cabecalho = {
        .status = '0', // Inconsistente até que seja fechado corretamente
        .no_raiz = -1,
        .topo = -1,
        .prox_RRN = 0,
        .nro_nos = 0};

    // Escreve o cabeçalho inicial no arquivo
    fwrite(&cabecalho.status, sizeof(char), 1, arquivo);
    fwrite(&cabecalho.no_raiz, sizeof(int), 1, arquivo);
    fwrite(&cabecalho.topo, sizeof(int), 1, arquivo);
    fwrite(&cabecalho.prox_RRN, sizeof(int), 1, arquivo);
    fwrite(&cabecalho.nro_nos, sizeof(int), 1, arquivo);

    return arquivo;
}

FILE *arvore_b_abrir_leitura(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "rb+");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo de índice");
        exit(EXIT_FAILURE);
    }
    return arquivo;
}

no_arvore_b arvore_b_ler_no(FILE *arquivo, int rrn)
{
    no_arvore_b no;

    // Posiciona o ponteiro: Tamanho do cabecalho + (RRN * Tamanho do No)
    fseek(arquivo, TAM_CABECALHO + (rrn * TAM_NO), SEEK_SET);

    fread(&no.removido, sizeof(char), 1, arquivo);
    fread(&no.proximo, sizeof(int), 1, arquivo);
    fread(&no.tipo_no, sizeof(int), 1, arquivo);
    fread(&no.numero_chaves, sizeof(int), 1, arquivo);

    // Chaves e Referencias (Intercalados C1, PR1, C2, PR2...)
    for (int i = 0; i < MAX_CHAVES; i++)
    {
        fread(&no.chaves[i], sizeof(int), 1, arquivo);
        fread(&no.dados_byte_offsets[i], sizeof(int), 1, arquivo);
    }

    // Ponteiros das Subarvores (P1, P2, P3, P4)
    for (int i = 0; i < ORDEM; i++)
    {
        fread(&no.filhos[i], sizeof(int), 1, arquivo);
    }

    return no;
}

void arvore_b_atualizar_cabecalho(FILE *arquivo, header_arvore_b *cabecalho)
{
    if (arquivo == NULL || cabecalho == NULL)
        return;

    fseek(arquivo, 0, SEEK_SET);
    fwrite(&cabecalho->status, sizeof(char), 1, arquivo);
    fwrite(&cabecalho->no_raiz, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->topo, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->prox_RRN, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->nro_nos, sizeof(int), 1, arquivo);
}

void arvore_b_escrever_no(FILE *arquivo, int rrn, no_arvore_b *no)
{
    // Posiciona o ponteiro: Tamanho do cabecalho + (RRN * Tamanho do No)
    fseek(arquivo, TAM_CABECALHO + (rrn * TAM_NO), SEEK_SET);

    // Escrita campo a campo obrigatoria conforme as restricoes
    fwrite(&no->removido, sizeof(char), 1, arquivo);
    fwrite(&no->proximo, sizeof(int), 1, arquivo);
    fwrite(&no->tipo_no, sizeof(int), 1, arquivo);
    fwrite(&no->numero_chaves, sizeof(int), 1, arquivo);

    // Chaves e Referencias (Intercalados C1, PR1, C2, PR2...)
    for (int i = 0; i < MAX_CHAVES; i++)
    {
        fwrite(&no->chaves[i], sizeof(int), 1, arquivo);
        fwrite(&no->dados_byte_offsets[i], sizeof(int), 1, arquivo);
    }

    // Ponteiros das Subarvores (P1, P2, P3, P4)
    for (int i = 0; i < ORDEM; i++)
    {
        fwrite(&no->filhos[i], sizeof(int), 1, arquivo);
    }
}

void arvore_b_fechar(FILE *arquivo, header_arvore_b *cabecalho)
{
    if (arquivo == NULL)
        return;

    // Atualiza status para consistente
    cabecalho->status = '1';

    // Reescreve o cabecalho
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&cabecalho->status, sizeof(char), 1, arquivo);
    fwrite(&cabecalho->no_raiz, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->topo, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->prox_RRN, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->nro_nos, sizeof(int), 1, arquivo);

    fclose(arquivo);
}

/**
 * @brief Função auxiliar interna que executa a busca na Árvore-B de forma recursiva.
 * @param arquivo Ponteiro para o arquivo binário do índice Árvore-B.
 * @param RRN_atual RRN do nó atual a ser inspecionado.
 * @param chave_busca O valor de codEstacao que está sendo procurado.
 * @return Retorna o ponteiro para o arquivo de dados (PR) se encontrado, ou -1 caso contrário.
 */
int arvore_b_buscar_recursivo(FILE *arquivo, int RRN_atual, int chave_busca)
{
    // Caso base: se o RRN for -1, atingiu uma subárvore inexistente (chave não encontrada)
    if (RRN_atual == -1)
    {
        return -1;
    }

    // Realiza a leitura física do nó atual a partir do disco
    no_arvore_b no = arvore_b_ler_no(arquivo, RRN_atual);

    int i = 0;
    // Percorre as chaves do nó enquanto a chave procurada for maior que as chaves locais
    while (i < no.numero_chaves && chave_busca > no.chaves[i])
    {
        i++;
    }

    // Caso 1: A chave foi encontrada no nó atual
    if (i < no.numero_chaves && chave_busca == no.chaves[i])
    {
        return no.dados_byte_offsets[i]; // Retorna o PR (ponteiro para o arquivo de dados)
    }

    // Caso 2: A chave não está neste nó e ele é um nó folha (tipo_no == -1)
    if (no.tipo_no == -1)
    {
        return -1; // Não há mais subárvores para descer
    }

    // Caso 3: Intermediário/Raiz. Continua a busca descendo para a subárvore correspondente
    return arvore_b_buscar_recursivo(arquivo, no.filhos[i], chave_busca);
}

/**
 * @brief Inicia a busca a partir do nó raiz da Árvore-B de forma gerencial.
 * @param arquivo Ponteiro para o arquivo binário do índice Árvore-B.
 * @param cabecalho Estrutura do cabeçalho da árvore para obter a raiz atual.
 * @param chave_busca O valor de codEstacao que está sendo procurado.
 * @return Retorna o PR se encontrado, ou -1 se a árvore estiver vazia ou não contiver a chave.
 */
int arvore_b_buscar(FILE *arquivo, header_arvore_b *cabecalho, int chave_busca)
{
    if (arquivo == NULL || cabecalho == NULL || cabecalho->no_raiz == -1)
    {
        return -1; // Árvore vazia ou parâmetros inválidos
    }

    // Dispara a recursão passando o RRN do nó raiz
    return arvore_b_buscar_recursivo(arquivo, cabecalho->no_raiz, chave_busca);
}

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
    if (no.tipo_no == -1) {
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
    if (RRN_atual == -1) return false; // Chave não encontrada

    no_arvore_b no = arvore_b_ler_no(arquivo, RRN_atual);
    

    // i é incrementado até encontrar a posição onde a chave buscada deveria estar (ou o final das chaves)
    int i = 0;
    while (i < no.numero_chaves && chave_busca > no.chaves[i]) {
        i++;
    }

    bool underflow_filho = false;

    // CASO 1: A chave foi encontrada neste nó
    if (i < no.numero_chaves && chave_busca == no.chaves[i]) {
        if (no.tipo_no == -1) {
            // É folha: Remove fazendo shift para a esquerda
            for (int j = i; j < no.numero_chaves - 1; j++) {
                no.chaves[j] = no.chaves[j + 1];
                no.dados_byte_offsets[j] = no.dados_byte_offsets[j + 1];
            }
            no.chaves[no.numero_chaves - 1] = -1;
            no.dados_byte_offsets[no.numero_chaves - 1] = -1;
            no.numero_chaves--;
            
            arvore_b_escrever_no(arquivo, RRN_atual, &no);
            return (no.numero_chaves == 0); // Ordem 4: Mínimo é 1. Se zerou, deu underflow
        } else {
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
    } else {
        // CASO 2: A chave não está neste nó, desce recursivamente
        underflow_filho = _remover_recursivo(arquivo, cabecalho, no.filhos[i], chave_busca);
    }

    // =========================================================================
    // TRATAMENTO DE UNDERFLOW NO FILHO [i]
    // =========================================================================
    if (underflow_filho) {
        int rrn_filho = no.filhos[i];
        no_arvore_b filho = arvore_b_ler_no(arquivo, rrn_filho);

        bool tem_irmao_dir = (i < no.numero_chaves);
        bool tem_irmao_esq = (i > 0);

        no_arvore_b irmao_dir, irmao_esq;
        int rrn_irmao_dir = -1, rrn_irmao_esq = -1;

        if (tem_irmao_dir) {
            rrn_irmao_dir = no.filhos[i + 1];
            irmao_dir = arvore_b_ler_no(arquivo, rrn_irmao_dir);
        }
        if (tem_irmao_esq) {
            rrn_irmao_esq = no.filhos[i - 1];
            irmao_esq = arvore_b_ler_no(arquivo, rrn_irmao_esq);
        }

        if (tem_irmao_dir && irmao_dir.numero_chaves > 1) {
            filho.chaves[0] = no.chaves[i];
            filho.dados_byte_offsets[0] = no.dados_byte_offsets[i];
            filho.filhos[1] = irmao_dir.filhos[0];
            filho.numero_chaves = 1;

            no.chaves[i] = irmao_dir.chaves[0];
            no.dados_byte_offsets[i] = irmao_dir.dados_byte_offsets[0];

            irmao_dir.filhos[0] = irmao_dir.filhos[1];
            for (int j = 0; j < irmao_dir.numero_chaves - 1; j++) {
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

        if (tem_irmao_esq && irmao_esq.numero_chaves > 1) {
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

        if (tem_irmao_esq) {
            // Irmão Esquerdo absorve a chave do Pai e todo o conteúdo do Filho (que estava vazio de chaves)
            irmao_esq.chaves[irmao_esq.numero_chaves] = no.chaves[i - 1];
            irmao_esq.dados_byte_offsets[irmao_esq.numero_chaves] = no.dados_byte_offsets[i - 1];
            irmao_esq.filhos[irmao_esq.numero_chaves + 1] = filho.filhos[0];
            irmao_esq.numero_chaves++; 

            // A página do Filho (à direita da junção) é destruída
            _remover_pagina(arquivo, cabecalho, rrn_filho);

            // Shift no Pai para remover a chave [i-1] que desceu e o ponteiro [i] que apontava para o Filho morto
            for (int j = i - 1; j < no.numero_chaves - 1; j++) {
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

        if (tem_irmao_dir) {
            // O Filho (atual, sem chaves) absorve a chave do Pai e todo o conteúdo do Irmão Direito
            filho.chaves[0] = no.chaves[i];
            filho.dados_byte_offsets[0] = no.dados_byte_offsets[i];
            filho.filhos[1] = irmao_dir.filhos[0];
            
            // Traz as chaves e ponteiros do irmão direito para o filho
            for(int j = 0; j < irmao_dir.numero_chaves; j++) {
                filho.chaves[1 + j] = irmao_dir.chaves[j];
                filho.dados_byte_offsets[1 + j] = irmao_dir.dados_byte_offsets[j];
                filho.filhos[2 + j] = irmao_dir.filhos[j + 1];
            }
            filho.numero_chaves = 1 + irmao_dir.numero_chaves; 

            // A página do Irmão Direito é destruída
            _remover_pagina(arquivo, cabecalho, rrn_irmao_dir);

            // Shift no Pai para remover a chave [i] que desceu e o ponteiro [i+1]
            for (int j = i; j < no.numero_chaves - 1; j++) {
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
    if (arquivo == NULL || cabecalho == NULL || cabecalho->no_raiz == -1) return;

    bool underflow_raiz = _remover_recursivo(arquivo, cabecalho, cabecalho->no_raiz, chave_busca);

    // Se a raiz sofreu underflow e zerou suas chaves
    if (underflow_raiz) {
        no_arvore_b raiz = arvore_b_ler_no(arquivo, cabecalho->no_raiz);
        
        // Se ela tiver um descendente, esse descendente vira a nova raiz absoluta
        if (raiz.tipo_no != -1) {
            int nova_raiz_rrn = raiz.filhos[0];
            _remover_pagina(arquivo, cabecalho, cabecalho->no_raiz);
            cabecalho->no_raiz = nova_raiz_rrn;
        } else {
            // Se a raiz folha secou, a árvore está 100% vazia
            _remover_pagina(arquivo, cabecalho, cabecalho->no_raiz);
            cabecalho->no_raiz = -1;
        }
    }

    // Assim como na inserção, é crucial gravar o cabeçalho no disco ao fim
    arvore_b_atualizar_cabecalho(arquivo, cabecalho);
}
