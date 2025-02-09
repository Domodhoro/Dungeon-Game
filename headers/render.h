#ifndef RENDER_H
#define RENDER_H

// Criação e configuração do renderizador.
_Bool createRenderer(Game *game) {
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (not game->renderer) {
        // Caso ocorra erro ao criar o renderizador, exibe a mensagem de erro.
        SDL_Log("Falha ao criar o renderizador: %s.\n", SDL_GetError());
        return false;
    }

    // Define a cor de fundo.
    game->backgroundColor = GRAY;

    // Define as dimensões da janela de visualização do renderizador.
    game->viewport = (SDL_Rect) {
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
    };
    return true;
}

// Função responsável por renderizar o conteúdo na tela.
void render(Game *game) {
    // Define a cor de fundo da tela.
    SDL_SetRenderDrawColor(game->renderer, game->backgroundColor.r, game->backgroundColor.g, game->backgroundColor.b, 255);
    SDL_RenderClear(game->renderer);

    // Renderiza os elementos do jogo com base no estado do mesmo.
    switch (game->state) {
        case MAIN_MENU:
            renderMainMenu(game);
            break;
        case ACTIVE:
            renderBackgroundDungeon(game);
            renderPlayer(game);
            renderForegroundDungeon(game);
            renderInventory(game);
            break;
        case PAUSE:

            break;
    }
    
    // Apresenta o conteúdo renderizado na tela.
    SDL_RenderSetViewport(game->renderer, &game->viewport);
    SDL_RenderPresent(game->renderer);
}

#endif // RENDER_H