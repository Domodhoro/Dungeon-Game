#ifndef FINISH_H
#define FINISH_H

// Função que destrói as texturas do jogo.
static void destroyTexture(Game *game) {
    if (game->mainMenu.texture) {
        SDL_DestroyTexture(game->mainMenu.texture);
        game->mainMenu.texture = NULL;
    }
    if (game->mainMenu.backgroundTexture) {
        SDL_DestroyTexture(game->mainMenu.backgroundTexture);
        game->mainMenu.backgroundTexture = NULL;
    }
    for (int i = 0; i < 2; i++) {
        if (game->mainMenu.button[i].text.texture) {
            SDL_DestroyTexture(game->mainMenu.button[i].text.texture);
            game->mainMenu.button[i].text.texture = NULL;
        }
    }
    if (game->mainMenu.version.texture) {
        SDL_DestroyTexture(game->mainMenu.version.texture);
        game->mainMenu.version.texture = NULL;
    }
    if (game->player.texture) {
        SDL_DestroyTexture(game->player.texture);
        game->player.texture = NULL;
    }
    if (game->dungeon.texture) {
        SDL_DestroyTexture(game->dungeon.texture);
        game->dungeon.texture = NULL;
    }
    if (game->inventory.texture) {
        SDL_DestroyTexture(game->inventory.texture);
        game->inventory.texture = NULL;
    }
}

// Função que destrói a fonte do jogo.
static void destroyFont(Game *game) {
    if (game->font[0]) {
        TTF_CloseFont(game->font[0]);
        game->font[0] = NULL;
    }
}

// Função responsável por encerrar o jogo e liberar todos os recursos alocados.
void finish(Game *game) {    
    // Verifica se os objetos do jogo existem e os destroem, liberando memória.
    destroyTexture(game);
    destroyFont(game);

    // Destrói o renderizador e a janela, liberando memória.
    if (game->renderer) {
        SDL_DestroyRenderer(game->renderer);
        game->renderer = NULL;
    }
    if (game->window) {
        SDL_DestroyWindow(game->window);
        game->window = NULL;
    }

    // Finaliza a bibliotecas do SDL.
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    // Fecha o estado lua.
    lua_close(game->L);
    game->L = NULL;
}

#endif // FINISH_H