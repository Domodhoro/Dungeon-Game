// Incluindo as bibliotecas e cabeçalhos necessários para o jogo.
#include "./headers/dependencies.h"

// Função principal.
int main(int argc, char *argv[]) {
    // Cria uma instância do jogo.
    Game *game = myMalloc(sizeof(Game));
    if (not game) {
        fprintf(stderr, "Falha ao alocar mémoria para a estrutura principal do jogo.\n");
        // Encerra o programa com falha.
        return -1;
    }
    // Zera a memória alocada para a estrutura do jogo.
    memset(game, 0, sizeof(Game));

    // Inicializa o jogo, se falhar, libera a memória e encerra o programa.
    if (not init(game)) {
        free(game);
        game = NULL;
        // Encerra o programa com falha.
        return -1;
    }

    // Inicia o jogo e entra na laço principal.
    game->isRunning = true;
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
    } while (game->isRunning);

    // Finaliza o jogo.
    finish(game);

    // Libera a memória alocada para a estrutura do jogo.
    myFree(game);
    game = NULL;
    
#if DEV
    // Verificando se houve vazamento de memória.
    printf("mallocCounter = %d.\n", getMallocCount());
#endif // DEV
    // Encerra o programa com sucesso.
    return 0;
}

// https://www.youtube.com/watch?v=5TitKidMhIc