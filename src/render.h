#ifndef RENDER_H
#define RENDER_H

// Função responsável por renderizar o conteúdo na tela.
void render(Game *game) {
	// Define a cor de fundo da tela.
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);

    // Desenha a masmorra.
    int i = 0;
    int j = 0;
    for (i = 0; i != DUNGEON_WIDTH; i++) {
        for (j = 0; j != DUNGEON_HEIGHT; j++) {
        	if (game->dungeon.data[i][j] != AIR) {
        		game->dungeon.dst = (SDL_Rect) {
        			game->camera.position.x + i * 32, game->camera.position.y + j * 32, 32, 32
        		};
        		if (game->dungeon.data[i][j] == WOOD) {
	            	game->dungeon.src = (SDL_Rect) {
	            		0, 0, 32, 32
	            	};
	            } else if (game->dungeon.data[i][j] == STONE) {
	            	game->dungeon.src = (SDL_Rect) {
	            		32, 0, 32, 32
	            	};
	            }
	            SDL_RenderCopy(game->renderer, game->dungeon.texture, &game->dungeon.src, &game->dungeon.dst);
        	}
        }
    }

    // Desenha o jogador.
    SDL_RenderCopy(game->renderer, game->player.texture, &game->player.src, &game->player.dst);
    
    // Apresenta o conteúdo renderizado na tela.
    SDL_RenderPresent(game->renderer);
}

#endif // RENDER_H