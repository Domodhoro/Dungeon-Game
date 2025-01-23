// Incluindo as bibliotecas e cabeçalhos necessários para o jogo.
#include "./src/dependencies.h"

// Função principal.
int main(int argc, char *argv[]) {
    // Aloca memória para a estrutura principal de dados do jogo.
    Game *game = malloc(sizeof(Game));
    if (!game) {
        fprintf(stderr, "Falha ao alocar mémoria para o jogo.\n");
        return -1;
    }
    // Zera a memória alocada para a estrutura principal.
    memset(game, 0, sizeof(Game));

    // Inicializa o jogo, se falhar, libera a memória e encerra o programa.
    game->running = init(game);
    if (!game->running) {
        free(game);
        game = NULL;
        return -1;
    }

    // Loop principal do jogo.
    while (game->running) {
        // Marca o início do frame atual.
        game->frameStart = SDL_GetTicks();
        // Processa os eventos, atualiza a lógica do jogo e renderiza a cena atual.
        handleEvents(game);
        update(game);
        render(game);
        // Calcula o tempo gasto no processamento do frame.
        game->frameTime = SDL_GetTicks() - game->frameStart;
        // Se o tempo do frame for menor que o tempo alvo (FRAME_TIME), espera o tempo restante.
        if (game->frameTime < (Uint32)FRAME_TIME) {
            SDL_Delay((Uint32)FRAME_TIME - game->frameTime);
        }
    }

    // Termina o jogo e libera recursos alocados.
    finish(game);
    // Libera a memória alocada para a estrutura do jogo.
    if (game) {
        free(game);
        game = NULL;
    }
    // Encerra o programa com sucesso.
    return 0;
}