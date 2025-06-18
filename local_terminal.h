#ifndef LOCAL_TERMINAL_H
#define LOCAL_TERMINAL_H

#include "tipos_essenciais.h"
#include "sistema_infra.h"

EstadoBooleano registrarNovoPontoTerminal(RedeTransporteAereo* rede, const char* identificador, const char* localizacao);

I32 buscarIndicePontoTerminal(RedeTransporteAereo* rede, const char* identificador);

#endif