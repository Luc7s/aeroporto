#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema_infra.h"

#define CAPACIDADE_INICIAL_DA_REDE 5

/**
 * @brief Inicializa a estrutura da rede de transporte aéreo.
 *
 * Aloca a memória inicial para o vetor de pontos terminais e para a matriz de adjacência (mapaRotas).
 * A matriz é inicializada com zeros, indicando a ausência de rotas.
 * @param rede Ponteiro para a estrutura RedeTransporteAereo a ser inicializada.
 */
void inicializarRedeTransporte(RedeTransporteAereo* rede) {
    rede->numPontosAtivos = 0;
    rede->capacidadeAlocada = CAPACIDADE_INICIAL_DA_REDE;

    rede->pontosTerminais = (PontoTerminal*) malloc(sizeof(PontoTerminal) * rede->capacidadeAlocada);

    // Alocação da matriz de adjacência (mapaRotas)
    // A matriz representa as conexões diretas entre os pontos terminais.
    // mapaRotas[i][j] armazena o número da rota do terminal 'i' para o terminal 'j'.
    // Um valor 0 significa que não há rota direta.
    rede->mapaRotas = (U32**) malloc(sizeof(U32*) * rede->capacidadeAlocada);
    for (U32 i = 0; i < rede->capacidadeAlocada; i++) {
        rede->mapaRotas[i] = (U32*) calloc(rede->capacidadeAlocada, sizeof(U32)); // 'calloc' inicializa com zeros
    }
}

/**
 * @brief Libera toda a memória alocada dinamicamente para a rede de transporte.
 * @param rede Ponteiro para a estrutura RedeTransporteAereo cujos recursos serão liberados.
 */
void liberarRecursosRede(RedeTransporteAereo* rede) {
    for (U32 i = 0; i < rede->capacidadeAlocada; i++) {
        free(rede->mapaRotas[i]);
    }
    free(rede->mapaRotas);
    free(rede->pontosTerminais);
}