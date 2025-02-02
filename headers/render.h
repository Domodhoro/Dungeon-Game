#ifndef RENDER_H
#define RENDER_H

// Renderiza o menu principal.
static void renderMainMenu(Game *game) {
    // Renderiza o fundo do menu principal.
    SDL_RenderCopy(game->renderer, game->textures[MAIN_MENU_BACKGROUND_TEXTURE], NULL, &game->mainMenu.dst);
    // Renderiza os botões do menu principal.
    for (int i = 0; i < MAX_MAIN_MENU_BUTTONS; i++) {
        SDL_RenderCopy(game->renderer, game->textures[MAIN_MENU_TEXTURE], &game->mainMenu.button[i].src, &game->mainMenu.button[i].dst);
        SDL_RenderCopy(game->renderer, game->mainMenu.button[i].text.texture, NULL, &game->mainMenu.button[i].text.dst);
    }
    // Renderiza o texto com a versão do jogo.
    SDL_RenderCopy(game->renderer, game->mainMenu.version.texture, NULL, &game->mainMenu.version.dst);
}

// Renderiza o jogador.
static void renderPlayer(Game *game) {
    // Renderiza o jogador.
    SDL_RenderCopy(game->renderer, game->textures[PLAYER_TEXTURE], &game->player.src, &game->player.dst);
    // Renderiza os corações do jogador.
    SDL_RenderCopy(game->renderer, game->textures[HEART_TEXTURE], &game->player.hearts.src, &game->player.hearts.dst);
}

// Renderiza o fundo da masmorra.
static void renderBackgroundDungeon(Game *game) {
    for (int i = 0; i < DUNGEON_WIDTH; i++) {
        for (int j = 0; j < DUNGEON_HEIGHT; j++) {
            if (game->dungeon->block[i][j].type != AIR && game->dungeon->block[i][j].properties.isBackground) {
                // Renderiza os blocos da masmorra.
                SDL_RenderCopy(game->renderer, game->textures[DUNGEON_TEXTURE], &game->dungeon->block[i][j].properties.src, &game->dungeon->block[i][j].dst);   
            }
        }
    }
}

// Renderiza a frente da masmorra.
static void renderForegroundDungeon(Game *game) {
    for (int i = 0; i < DUNGEON_WIDTH; i++) {
        for (int j = 0; j < DUNGEON_HEIGHT; j++) {
            if (game->dungeon->block[i][j].type != AIR && !game->dungeon->block[i][j].properties.isBackground) {
                // Renderiza os blocos da masmorra.
                SDL_RenderCopy(game->renderer, game->textures[DUNGEON_TEXTURE], &game->dungeon->block[i][j].properties.src, &game->dungeon->block[i][j].dst);   
            }
        }
    }
}

// Renderiza o inventário do jogador.
static void renderInventory(Game *game) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        SDL_RenderCopy(game->renderer, game->textures[INVENTORY_TEXTURE], &game->inventory.src[i], &game->inventory.dst[i]);
    }
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