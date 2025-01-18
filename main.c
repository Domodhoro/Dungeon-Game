// Incluindo as bibliotecas e cabeçalhos necessários para o jogo.
#include "./src/dependencies.h"
#include "./src/defs.h"
#include "./src/enums.h"
#include "./src/structs.h"
#include "./src/render.h"
#include "./src/update.h"
#include "./src/handleEvents.h"
#include "./src/terminate.h"
#include "./src/init.h"

int main(int argc, char *argv[]) {
    // Aloca memória para a estrutura de dados do jogo.
    Game *game = malloc(sizeof(Game));
    if (!game) {
        fprintf(stderr, "Falha ao alocar mémoria para o jogo.\n");
        return -1;
    }
    // Zera a memória alocada para a estrutura do jogo.
    memset(game, 0, sizeof(Game));

    // Inicializa o jogo, se falhar, libera a memória e termina o programa.
    if (!init(game)) {
        free(game);
        return -1;
    }

    // Variáveis para controle do tempo de cada frame.
    Uint32 frameStart = 0;    
    Uint32 frameTime = 0;

    // Loop principal do jogo.
    game->running = true;
    while (game->running) {
        // Marca o início do frame atual.
        frameStart = SDL_GetTicks();
        // Processa os eventos, atualiza a lógica do jogo e renderiza a cena atual.
        handleEvents(game);
        update(game);
        render(game);
        // Calcula o tempo gasto no processamento do frame.
        frameTime = SDL_GetTicks() - frameStart;
        // Se o tempo do frame for menor que o tempo alvo (FRAME_TIME), espera o tempo restante.
        if (frameTime < (Uint32)FRAME_TIME) {
            SDL_Delay((Uint32)FRAME_TIME - frameTime);
        }
    }
    // Termina o jogo e libera recursos alocados.
    terminate(game);
    // Libera a memória alocada para a estrutura do jogo e encerra o programa com sucesso.
    if (game) {
        free(game);
    }
    return 0;
}