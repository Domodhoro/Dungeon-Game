#ifndef RENDER_H
#define RENDER_H

// Renderiza o menu principal.
static void renderMainMenu(Game *game) {
    // Renderiza o fundo do menu principal.
    SDL_RenderCopy(game->renderer, game->mainMenu.backgroundTexture, NULL, &game->mainMenu.dst);
    // Renderiza os botões do menu principal.
    for (int i = 0; i < MAX_MAIN_MENU_BUTTONS; i++) {
        SDL_RenderCopy(game->renderer, game->mainMenu.texture, &game->mainMenu.button[i].src, &game->mainMenu.button[i].dst);
        SDL_RenderCopy(game->renderer, game->mainMenu.button[i].text.texture, NULL, &game->mainMenu.button[i].text.dst);
    }
    // Renderiza o texto com a versão do jogo.
    SDL_RenderCopy(game->renderer, game->mainMenu.version.texture, NULL, &game->mainMenu.version.dst);
}

// Renderiza o jogador.
static void renderPlayer(Game *game) {
    SDL_RenderCopy(game->renderer, game->player.texture, &game->player.src, &game->player.dst);
    // Renderiza os corações do jogador.
    SDL_RenderCopy(game->renderer, game->player.hearts.texture, &game->player.hearts.src, &game->player.hearts.dst);
}

// Renderiza a masmorra.
static void renderDungeon(Game *game) {
    for (int i = 0; i < DUNGEON_WIDTH; i++) {
        for (int j = 0; j < DUNGEON_HEIGHT; j++) {
            if (game->dungeon.block[i][j].type != AIR) {
                // Renderiza os blocos da masmorra.
                SDL_RenderCopy(game->renderer, game->dungeon.texture, &game->dungeon.block[i][j].properties.src, &game->dungeon.block[i][j].dst);
            }
        }
    }
}

// Renderiza o inventário do jogador.
static void renderInventory(Game *game) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        SDL_RenderCopy(game->renderer, game->inventory.texture, &game->inventory.src[i], &game->inventory.dst[i]);
    }
}

// Renderiza um foco de luz sobre o jogador.
static void renderLight(Game *game) {
    // Habilita o blending.
    SDL_SetTextureBlendMode(game->light.texture, SDL_BLENDMODE_ADD);
    // Desenha um foco de luz no centro da tela.
    SDL_RenderCopy(game->renderer, game->light.texture, NULL, &game->light.dst);
    // Desabilita o blending.
    SDL_SetTextureBlendMode(game->light.texture, SDL_BLENDMODE_BLEND);
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
        renderDungeon(game);
        renderLight(game);
        renderPlayer(game);
        renderInventory(game);
        break;
    }
    
    // Apresenta o conteúdo renderizado na tela.
    SDL_RenderSetViewport(game->renderer, &game->viewport);
    SDL_RenderPresent(game->renderer);
}

#endif // RENDER_H