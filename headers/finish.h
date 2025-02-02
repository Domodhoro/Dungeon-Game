#ifndef FINISH_H
#define FINISH_H

// Função que destrói as texturas do jogo.
static void destroyTextures(Game *game) {
    for (int i = 0; i < MAX_TEXTURES; i++) {
        if (game->textures[i]) {
            SDL_DestroyTexture(game->textures[i]);
            game->textures[i] = NULL;
        }
    }
}

// Função que destrói a fonte do jogo.
static void destroyFonts(Game *game) {
    for (int i = 0; i < MAX_FONTS; i++) {
        if (game->font[i]) {
            TTF_CloseFont(game->font[i]);
            game->font[i] = NULL;
        }
    }
}

// Função responsável por encerrar o jogo e liberar todos os recursos alocados.
void finish(Game *game) {    
    // Verifica se os objetos do jogo existem e os destroem, liberando memória.
    destroyTextures(game);
    destroyFonts(game);

    // Destrói a masmorra, liberando memória.
    if (game->dungeon) {
        free(game->dungeon);
        game->dungeon = NULL;
    }

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
}

#endif // FINISH_H