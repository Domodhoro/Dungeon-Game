#ifndef RENDER_H
#define RENDER_H

// Função para renderizar o fundo da masmorra.
static void renderBackgroundDungeon(Game *game) {
    renderDungeonLayer(game, true);
}

// Função para renderizar a frente da masmorra.
static void renderForegroundDungeon(Game *game) {
    renderDungeonLayer(game, false);
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