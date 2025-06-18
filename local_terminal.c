#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "local_terminal.h"

EstadoBooleano registrarNovoPontoTerminal(RedeTransporteAereo* rede, const char* identificador, const char* localizacao) {
    if (buscarIndicePontoTerminal(rede, identificador) != -1) {
        return FALSO;
    }

    // Lógica de redimensionamento da matriz de adjacência e do vetor de terminais, se a capacidade for atingida.
    if (rede->numPontosAtivos >= rede->capacidadeAlocada) {
        U32 novaCapacidade = rede->capacidadeAlocada * 2;

        rede->pontosTerminais = realloc(rede->pontosTerminais, novaCapacidade * sizeof(PontoTerminal));

        for (U32 i = 0; i < rede->capacidadeAlocada; i++) {
            rede->mapaRotas[i] = realloc(rede->mapaRotas[i], novaCapacidade * sizeof(U32));
            for (U32 j = rede->capacidadeAlocada; j < novaCapacidade; j++) {
                rede->mapaRotas[i][j] = 0;
            }
        }

        rede->mapaRotas = realloc(rede->mapaRotas, novaCapacidade * sizeof(U32*));
        for (U32 i = rede->capacidadeAlocada; i < novaCapacidade; i++) {
            rede->mapaRotas[i] = calloc(novaCapacidade, sizeof(U32));
        }

        rede->capacidadeAlocada = novaCapacidade;
    }

    strncpy(rede->pontosTerminais[rede->numPontosAtivos].identificador, identificador, COMPRIMENTO_CODIGO);
    strncpy(rede->pontosTerminais[rede->numPontosAtivos].localizacao, localizacao, COMPRIMENTO_CIDADE);
    rede->numPontosAtivos++;

    return VERDADEIRO;
}

I32 buscarIndicePontoTerminal(RedeTransporteAereo* rede, const char* identificador) {
    for (U32 i = 0; i < rede->numPontosAtivos; i++) {
        if (strncmp(rede->pontosTerminais[i].identificador, identificador, 3) == 0) {
            return i;
        }
    }
    return -1;
}