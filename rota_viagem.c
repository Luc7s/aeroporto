#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rota_viagem.h"

EstadoBooleano registrarNovaRota(RedeTransporteAereo* rede, const char* origem_id, const char* destino_id, U32 numeroRota) {
    I32 indiceOrigem = buscarIndicePontoTerminal(rede, origem_id);
    I32 indiceDestino = buscarIndicePontoTerminal(rede, destino_id);

    if (indiceOrigem == -1 || indiceDestino == -1) {
        return FALSO;
    }

    if (rede->mapaRotas[indiceOrigem][indiceDestino] != 0) {
        return FALSO;
    }

    rede->mapaRotas[indiceOrigem][indiceDestino] = numeroRota; // Adiciona o número da rota na matriz de adjacência
    return VERDADEIRO;
}

EstadoBooleano removerRotaExistente(RedeTransporteAereo* rede, U32 numeroRota) {
    for (U32 i = 0; i < rede->numPontosAtivos; i++) {
        for (U32 j = 0; j < rede->numPontosAtivos; j++) {
            if (rede->mapaRotas[i][j] == numeroRota) {
                rede->mapaRotas[i][j] = 0; // Remove a rota da matriz de adjacência (define como 0)
                return VERDADEIRO;
            }
        }
    }
    return FALSO;
}

EstadoBooleano obterRotasDePartida(RedeTransporteAereo* rede, const char* origem_id, InformacaoRota** rotas, U32* quantidade) {
    I32 idxOrig = buscarIndicePontoTerminal(rede, origem_id);
    if (idxOrig < 0) {
        *rotas = NULL;
        *quantidade = 0;
        return FALSO;
    }
    U32 count = 0;
    for (U32 j = 0; j < rede->numPontosAtivos; j++) {
        if (rede->mapaRotas[idxOrig][j] != 0) { // Verifica rotas de saída na matriz
            count++;
        }
    }
    *quantidade = count;
    if (count == 0) {
        *rotas = NULL;
        return VERDADEIRO;
    }
    InformacaoRota* lista = (InformacaoRota*) malloc(sizeof(InformacaoRota) * count);
    if (lista == NULL) {
        *rotas = NULL;
        *quantidade = 0;
        return FALSO;
    }
    U32 k = 0;
    for (U32 j = 0; j < rede->numPontosAtivos; j++) {
        if (rede->mapaRotas[idxOrig][j] != 0) {
            lista[k].numeroIdentificador = rede->mapaRotas[idxOrig][j];
            lista[k].destinoIndice = j;
            k++;
        }
    }
    *rotas = lista;
    return VERDADEIRO;
}

void liberarInformacaoRotas(InformacaoRota* rotas) {
    free(rotas);
}

EstadoBooleano obterRotasComConexao(RedeTransporteAereo* rede, const char* origem_id, const char* destino_id, InformacaoRota** rotas, U32* quantidade) {
    I32 idxOrig = buscarIndicePontoTerminal(rede, origem_id);
    I32 idxDest = buscarIndicePontoTerminal(rede, destino_id);
    if (idxOrig < 0 || idxDest < 0) {
        *rotas = NULL;
        *quantidade = 0;
        return FALSO;
    }

    U32 count = 0;
    for (U32 j = 0; j < rede->numPontosAtivos; j++) {
        if (rede->mapaRotas[idxOrig][j] != 0 && rede->mapaRotas[j][idxDest] != 0) { // Verifica rotas com uma conexão
            count++;
        }
    }
    
    *quantidade = count;
    if (count == 0) {
        *rotas = NULL;
        return VERDADEIRO;
    }

    InformacaoRota* lista = (InformacaoRota*) malloc(sizeof(InformacaoRota) * count);
    if (lista == NULL) {
        *rotas = NULL;
        *quantidade = 0;
        return FALSO;
    }
    U32 k = 0;
    for (U32 j = 0; j < rede->numPontosAtivos; j++) {
        if (rede->mapaRotas[idxOrig][j] != 0 && rede->mapaRotas[j][idxDest] != 0) {
            lista[k].numeroIdentificador = rede->mapaRotas[idxOrig][j];
            lista[k].origemIndice = idxOrig;
            lista[k].destinoIndice = j;
            k++;
        }
    }
    
    *rotas = lista;
    return VERDADEIRO;
}

/**
 * @brief Função recursiva auxiliar para encontrar todos os caminhos entre dois pontos terminais.
 *
 * Utiliza o algoritmo de busca em profundidade (DFS) para explorar todas as rotas possíveis.
 * @param rede Ponteiro para a estrutura RedeTransporteAereo.
 * @param atual Índice do ponto terminal atual na busca.
 * @param destino Índice do ponto terminal de destino.
 * @param visitado Array booleano para marcar terminais já visitados no caminho atual (evita ciclos).
 * @param caminho Array para armazenar a sequência de índices de terminais no caminho atual.
 * @param nivel Nível de profundidade na recursão (comprimento do caminho atual).
 * @param total Ponteiro para um contador que registra o número total de caminhos encontrados.
 */
static void procurarCaminhosRecursivo(RedeTransporteAereo* rede, I32 atual, I32 destino, EstadoBooleano* visitado, I32* caminho, U32 nivel, U32* total) {
    visitado[atual] = VERDADEIRO;
    caminho[nivel] = atual;

    if (atual == destino) {
        printf("  > Rota %u: ", ++(*total));
        for (U32 i = 0; i < nivel; i++) {
            printf("%s (Rota %u) -> ", rede->pontosTerminais[caminho[i]].identificador, rede->mapaRotas[caminho[i]][caminho[i+1]]);
        }
        printf("%s\n", rede->pontosTerminais[caminho[nivel]].identificador);
    } else {
        for (U32 prox = 0; prox < rede->numPontosAtivos; prox++) {
            if (!visitado[prox] && rede->mapaRotas[atual][prox] != 0) { // Verifica se há rota direta na matriz
                procurarCaminhosRecursivo(rede, prox, destino, visitado, caminho, nivel + 1, total);
            }
        }
    }
    visitado[atual] = FALSO;
}

/**
 * @brief Lista todos os possíveis caminhos (trajetos) entre dois pontos terminais na rede.
 *
 * Encontra e imprime todas as sequências de rotas que conectam um terminal de origem a um de destino.
 * @param rede Ponteiro para a estrutura RedeTransporteAereo.
 * @param origem_id Identificador do ponto terminal de partida.
 * @param destino_id Identificador do ponto terminal de chegada.
 */
void listarTodosCaminhosNaRede(RedeTransporteAereo* rede, const char* origem_id, const char* destino_id) {
    I32 idxOrig = buscarIndicePontoTerminal(rede, origem_id);
    I32 idxDest = buscarIndicePontoTerminal(rede, destino_id);
    if (idxOrig < 0 || idxDest < 0) {
        printf("Erro: Um ou ambos os pontos terminais nao existem na rede.\n");
        return;
    }
    
    EstadoBooleano* visitado = (EstadoBooleano*) calloc(rede->numPontosAtivos, sizeof(EstadoBooleano));
    I32* caminho = (I32*) malloc(rede->numPontosAtivos * sizeof(I32));

    if (!visitado || !caminho) {
        printf("Erro de alocacao de memoria para o calculo de rotas.\n");
        free(visitado);
        free(caminho);
        return;
    }

    U32 total = 0;
    printf("\nIdentificando Rotas de '%s' para '%s':\n", origem_id, destino_id);
    procurarCaminhosRecursivo(rede, idxOrig, idxDest, visitado, caminho, 0, &total);

    if (total == 0) {
        printf("  Nenhuma rota direta ou indireta encontrada entre os pontos.\n");
    }

    free(visitado);
    free(caminho);
}