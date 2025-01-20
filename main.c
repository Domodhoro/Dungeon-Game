// Incluindo as bibliotecas e cabeçalhos necessários para o jogo.
#include "./src/dependencies.h"

// Função principal.
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
    game->frameStart = 0;    
    game->frameTime = 0;
    // Loop principal do jogo.
    game->running = true;
    do {
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
    } while (game->running);
    // Termina o jogo e libera recursos alocados.
    finish(game);
    // Libera a memória alocada para a estrutura do jogo e encerra o programa com sucesso.
    if (game) {
        free(game);
        game = NULL;
    }
    return 0;
}