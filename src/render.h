#ifndef RENDER_H
#define RENDER_H

// Renderiza o jogador.
static void renderPlayer(Game *game) {
    SDL_RenderCopy(game->renderer, game->player->texture, &game->player->src, &game->player->dst);
}

// Renderiza a masmorra.
static void renderDungeon(Game *game) {
    int i;
    int j;
    for (i = 0; i < DUNGEON_WIDTH; i++) {
        for (j = 0; j < DUNGEON_HEIGHT; j++) {
            if (game->dungeon->block[i][j].type != AIR) {
                if (game->dungeon->block[i][j].type == WOOD) {
                    game->dungeon->block[i][j].src = (SDL_Rect) {
                        0, 0, 32, 32
                    };
                } else if (game->dungeon->block[i][j].type == STONE) {
                    game->dungeon->block[i][j].src = (SDL_Rect) {
                        32, 0, 32, 32
                    };
                }
                // Renderiza os blocos da masmorra.
                SDL_RenderCopy(game->renderer, game->dungeon->texture, &game->dungeon->block[i][j].src, &game->dungeon->block[i][j].dst);
            }
        }
    }
}

// Função responsável por renderizar o conteúdo na tela.
void render(Game *game) {
	// Define a cor de fundo da tela.
    SDL_SetRenderDrawColor(game->renderer, 100, 100, 100, 255);
    SDL_RenderClear(game->renderer);

    // Renderiza o jogo.
    renderDungeon(game);
    renderPlayer(game);
    
    // Apresenta o conteúdo renderizado na tela.
    SDL_RenderSetViewport(game->renderer, &game->viewport);
    SDL_RenderPresent(game->renderer);
}

#endif // RENDER_H