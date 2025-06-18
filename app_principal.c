#include "tipos_essenciais.h"
#include "sistema_infra.h"
#include "local_terminal.h"
#include "rota_viagem.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void limpar_entrada_buffer();
void exibir_opcoes_menu();
void gerenciar_cadastro_terminal(RedeTransporteAereo* rede);
void gerenciar_cadastro_rota(RedeTransporteAereo* rede);
void gerenciar_remocao_rota(RedeTransporteAereo* rede);
void gerenciar_listagem_partidas(RedeTransporteAereo* rede);
void gerenciar_listagem_trajetos(RedeTransporteAereo* rede);

/**
 * @brief Função principal do programa.
 *
 * Inicializa a rede de transporte, cadastra dados iniciais de terminais e rotas,
 * e entra no loop principal do menu para interação com o usuário.
 * Gerencia a escolha de operações e libera os recursos alocados ao finalizar.
 * @return Retorna 0 ao finalizar o programa com sucesso.
 */
I32 main() {
    int escolha_operacao;
    RedeTransporteAereo redeAereaPrincipal;

    inicializarRedeTransporte(&redeAereaPrincipal);

    printf("\n********************************************************\n");
    printf("* *\n");
    printf("* BEM-VINDO AO GESTOR DE REDE DE TRANSPORTE            *\n");
    printf("* *\n");
    printf("********************************************************\n\n");

    printf(">>> Configurando pontos terminais iniciais...\n");
    struct { char cod_id[COMPRIMENTO_CODIGO]; char nome_local[COMPRIMENTO_CIDADE]; } pontos_iniciais[] = {
        {"POA", "Porto Alegre"},
        {"REC", "Recife"},
        {"SDU", "Rio de Janeiro (Santos Dumont)"},
        {"SSA", "Salvador"},
        {"CWB", "Curitiba"}
    };
    for (int i = 0; i < 5; i++) {
        if (registrarNovoPontoTerminal(&redeAereaPrincipal, pontos_iniciais[i].cod_id, pontos_iniciais[i].nome_local)) {
            printf("  [OK] Ponto Terminal %s (%s) adicionado.\n", pontos_iniciais[i].cod_id, pontos_iniciais[i].nome_local);
        } else {
            printf("  [ERRO] Falha ao adicionar Ponto Terminal %s (%s).\n", pontos_iniciais[i].cod_id, pontos_iniciais[i].nome_local);
        }
    }
    printf("\n>>> Estabelecendo rotas de viagem iniciais...\n");

    struct { char origem_id[COMPRIMENTO_CODIGO]; char destino_id[COMPRIMENTO_CODIGO]; U32 num_rota; } rotas_iniciais[] = {
        {"POA", "REC", 100},
        {"REC", "SDU", 200},
        {"SDU", "CWB", 300},
        {"CWB", "SSA", 400},
        {"SSA", "POA", 500},
        {"POA", "CWB", 150},
        {"REC", "CWB", 250},
        {"SDU", "POA", 350},
        {"SSA", "SDU", 450}
    };
    for (int i = 0; i < sizeof(rotas_iniciais)/sizeof(rotas_iniciais[0]); i++) {
        if (registrarNovaRota(&redeAereaPrincipal, rotas_iniciais[i].origem_id, rotas_iniciais[i].destino_id, rotas_iniciais[i].num_rota)) {
            printf("  [OK] Rota %u estabelecida: %s -> %s\n", rotas_iniciais[i].num_rota, rotas_iniciais[i].origem_id, rotas_iniciais[i].destino_id);
        } else {
             printf("  [ERRO] Falha ao estabelecer Rota %u: %s -> %s (verifique existencia ou duplicidade)\n", rotas_iniciais[i].num_rota, rotas_iniciais[i].origem_id, rotas_iniciais[i].destino_id);
        }
    }
    printf("\n");

    do {
        exibir_opcoes_menu();
        scanf("%d", &escolha_operacao);
        limpar_entrada_buffer();

        switch (escolha_operacao) {
            case 1:
                gerenciar_cadastro_terminal(&redeAereaPrincipal);
                break;
            case 2:
                gerenciar_cadastro_rota(&redeAereaPrincipal);
                break;
            case 3:
                gerenciar_remocao_rota(&redeAereaPrincipal);
                break;
            case 4:
                gerenciar_listagem_partidas(&redeAereaPrincipal);
                break;
            case 5:
                gerenciar_listagem_trajetos(&redeAereaPrincipal);
                break; 
            case 0:
                printf("\n--- Encerrando o Sistema de Gestao de Rede de Transporte. Ate mais! ---\n");
                break;
            default:
                printf("\n!!! Opcao invalida. Por favor, selecione um numero valido do menu. !!!\n");
        }
        if (escolha_operacao != 0) {
            printf("\nPressione ENTER para continuar...\n");
            limpar_entrada_buffer();
        }
    } while (escolha_operacao != 0);

    liberarRecursosRede(&redeAereaPrincipal);
    return 0;
}

/**
 * @brief Limpa o buffer de entrada do teclado.
 *
 * Lê e descarta os caracteres restantes no buffer de entrada (geralmente após um scanf),
 * incluindo o caractere de nova linha, para evitar que afetem leituras futuras.
 */
void limpar_entrada_buffer() {
    I32 c;
    while ((c = getchar()) != '\n' && c != EOF);
} 

/**
 * @brief Exibe o menu principal de operações para o usuário.
 *
 * Imprime na tela um menu formatado com as opções de gerenciamento da rede de transporte.
 */
void exibir_opcoes_menu() {
    printf("=========================================================\n");
    printf("|             MENU PRINCIPAL DE OPERACOES               |\n");
    printf("=========================================================\n");
    printf("| 1. Registrar Novo Ponto Terminal                      |\n");
    printf("| 2. Estabelecer Nova Rota de Viagem                    |\n");
    printf("| 3. Desfazer Rota de Viagem Existente                  |\n");
    printf("| 4. Visualizar Rotas de Partida de um Terminal         |\n");
    printf("| 5. Encontrar Trajetos Completos Entre Terminais       |\n");
    printf("| 0. SAIR DO PROGRAMA                                   |\n");
    printf("=========================================================\n");
    printf(">> Sua escolha: ");
}

/**
 * @brief Lida com a interação do usuário para registrar um novo ponto terminal.
 *
 * Solicita ao usuário o identificador (código) e o nome da cidade para o novo terminal,
 * e então chama a função de registro na rede. Exibe o resultado da operação.
 * @param rede Ponteiro para a estrutura RedeTransporteAereo.
 */
void gerenciar_cadastro_terminal(RedeTransporteAereo* rede) {
    char id_ponto[COMPRIMENTO_CODIGO];
    char nome_cidade_local[COMPRIMENTO_CIDADE];

    printf("\n--- Registro de Novo Ponto Terminal ---\n");
    printf("   > Identificador do Terminal (3 letras): ");
    scanf("%3s", id_ponto);
    limpar_entrada_buffer();

    printf("   > Nome da Cidade/Localizacao: ");
    scanf("%99[^\n]", nome_cidade_local);
    limpar_entrada_buffer();

    if (registrarNovoPontoTerminal(rede, id_ponto, nome_cidade_local)) {
        printf("\n   [SUCESSO] Ponto Terminal '%s' em '%s' registrado com sucesso.\n", id_ponto, nome_cidade_local);
    } else {
        printf("\n   [FALHA] Erro: Terminal com o identificador '%s' ja existe ou dados invalidos.\n", id_ponto);
    }
}

/**
 * @brief Lida com a interação do usuário para estabelecer uma nova rota de viagem.
 *
 * Solicita os identificadores dos terminais de origem e destino, e o número da rota.
 * Em seguida, tenta registrar esta rota na rede, exibindo o status da operação.
 * @param rede Ponteiro para a estrutura RedeTransporteAereo.
 */
void gerenciar_cadastro_rota(RedeTransporteAereo* rede) {
    char id_origem[COMPRIMENTO_CODIGO];
    char id_destino[COMPRIMENTO_CODIGO];
    U32 identificador_rota;

    printf("\n--- Estabelecer Nova Rota de Viagem ---\n");
    printf("   > Identificador do Ponto Terminal de Origem: ");
    scanf("%3s", id_origem);
    limpar_entrada_buffer();

    printf("   > Identificador do Ponto Terminal de Destino: ");
    scanf("%3s", id_destino);
    limpar_entrada_buffer();

    printf("   > Numero Identificador da Rota (ex: 123): ");
    scanf("%u", &identificador_rota);
    limpar_entrada_buffer();

    if (registrarNovaRota(rede, id_origem, id_destino, identificador_rota)) {
        printf("\n   [SUCESSO] Rota %u de '%s' para '%s' estabelecida na rede.\n", identificador_rota, id_origem, id_destino);
    } else {
        printf("\n   [FALHA] Erro: Verifique se os pontos terminais existem e se a rota ja esta registrada.\n");
    }
}

/**
 * @brief Lida com a interação do usuário para remover uma rota de viagem existente.
 *
 * Solicita o número identificador da rota a ser removida.
 * Tenta remover a rota da rede de transporte, exibindo o status da operação.
 * @param rede Ponteiro para a estrutura RedeTransporteAereo.
 */
void gerenciar_remocao_rota(RedeTransporteAereo* rede) {
    U32 identificador_rota;
    printf("\n--- Desfazer Rota de Viagem Existente ---\n");
    printf("   > Numero Identificador da Rota a ser removida: ");
    scanf("%u", &identificador_rota);
    limpar_entrada_buffer();

    if (removerRotaExistente(rede, identificador_rota)) {
        printf("\n   [SUCESSO] Rota %u removida da rede.\n", identificador_rota);
    } else {
        printf("\n   [FALHA] Erro: Rota %u nao encontrada na rede.\n", identificador_rota);
    }
}

/**
 * @brief Lida com a interação do usuário para visualizar as rotas de partida de um terminal.
 *
 * Solicita o identificador do terminal de origem.
 * Obtém e exibe todas as rotas que partem desse terminal, ou uma mensagem se não houver rotas.
 * @param rede Ponteiro para a estrutura RedeTransporteAereo.
 */
void gerenciar_listagem_partidas(RedeTransporteAereo* rede) {
    char id_origem[COMPRIMENTO_CODIGO];
    printf("\n--- Visualizar Rotas de Partida de um Terminal ---\n");
    printf("   > Identificador do Ponto Terminal de Origem: ");
    scanf("%3s", id_origem);
    limpar_entrada_buffer();

    InformacaoRota* lista_partidas;
    U32 quantidade_rotas;

    if (!obterRotasDePartida(rede, id_origem, &lista_partidas, &quantidade_rotas)) {
        printf("\n   [ATENCAO] Ponto Terminal '%s' nao encontrado.\n", id_origem);
        return;
    }
    if (quantidade_rotas == 0) {
        printf("\n   Nenhuma rota de partida registrada do ponto terminal '%s'.\n", id_origem);
        return;
    }
    printf("\nRotas de Partida confirmadas de '%s':\n", id_origem);
    printf("---------------------------------------------------------\n");
    for (U32 i = 0; i < quantidade_rotas; i++) {
        printf("   - Rota %u para '%s' (Identificador: %s)\n",
               lista_partidas[i].numeroIdentificador,
               rede->pontosTerminais[lista_partidas[i].destinoIndice].localizacao,
               rede->pontosTerminais[lista_partidas[i].destinoIndice].identificador);
    }
    printf("---------------------------------------------------------\n");
    liberarInformacaoRotas(lista_partidas);
}

/**
 * @brief Lida com a interação do usuário para encontrar e listar todos os trajetos entre dois terminais.
 *
 * Solicita os identificadores dos terminais de partida e chegada.
 * Chama a função que busca e exibe todos os caminhos possíveis (diretos e indiretos) na rede.
 * @param rede Ponteiro para a estrutura RedeTransporteAereo.
 */
void gerenciar_listagem_trajetos(RedeTransporteAereo* rede) {
    char id_origem_trajeto[COMPRIMENTO_CODIGO];
    char id_destino_trajeto[COMPRIMENTO_CODIGO];
    printf("\n--- Encontrar Trajetos Completos Entre Terminais ---\n");
    printf("   > Identificador do Ponto Terminal de Partida: ");
    scanf("%3s", id_origem_trajeto);
    limpar_entrada_buffer();

    printf("   > Identificador do Ponto Terminal de Chegada: ");
    scanf("%3s", id_destino_trajeto);
    limpar_entrada_buffer();

    printf("\nBuscando possiveis Trajetos de '%s' para '%s':\n", id_origem_trajeto, id_destino_trajeto);
    printf("---------------------------------------------------------\n");
    listarTodosCaminhosNaRede(rede, id_origem_trajeto, id_destino_trajeto);
    printf("---------------------------------------------------------\n");
}