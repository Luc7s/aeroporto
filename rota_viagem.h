#ifndef ROTA_VIAGEM_H
#define ROTA_VIAGEM_H

#include <stdlib.h>
#include <string.h>

#include "sistema_infra.h"
#include "tipos_essenciais.h"
#include "local_terminal.h"

EstadoBooleano registrarNovaRota(RedeTransporteAereo* rede, const char* origem_id, const char* destino_id, U32 numeroRota);

EstadoBooleano removerRotaExistente(RedeTransporteAereo* rede, U32 numeroRota);

typedef struct {
    I32 origemIndice;
    U32 numeroIdentificador;
    I32 destinoIndice;
} InformacaoRota;

EstadoBooleano obterRotasDePartida(RedeTransporteAereo* rede, const char* origem_id, InformacaoRota** rotas, U32* quantidade);

EstadoBooleano obterRotasComConexao(RedeTransporteAereo* rede, const char* origem_id, const char* destino_id, InformacaoRota** rotas, U32* quantidade);

void listarTodosCaminhosNaRede(RedeTransporteAereo* rede, const char* origem_id, const char* destino_id);

#endif