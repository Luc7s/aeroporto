#ifndef SISTEMA_INFRA_H
#define SISTEMA_INFRA_H

#include "tipos_essenciais.h"

#define COMPRIMENTO_CODIGO 4
#define COMPRIMENTO_CIDADE 100

typedef struct{
    char identificador[COMPRIMENTO_CODIGO];
    char localizacao[COMPRIMENTO_CIDADE];
} PontoTerminal;

/**
 * @brief Representa a rede de transporte aéreo usando um grafo.
 *
 * Esta estrutura contém os pontos terminais (vértices) e as rotas (arestas)
 * representadas por uma matriz de adjacência.
 */
typedef struct {
    PontoTerminal* pontosTerminais;   // Vetor dinâmico de pontos terminais (vértices do grafo)
    U32** mapaRotas;                  // Matriz de adjacência (mapaRotas[origem][destino] = número da rota, ou 0 se não houver rota)
    U32 numPontosAtivos;
    U32 capacidadeAlocada;
} RedeTransporteAereo;

void inicializarRedeTransporte(RedeTransporteAereo* rede);
void liberarRecursosRede(RedeTransporteAereo* rede);

#endif