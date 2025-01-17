#ifndef TERMINATE_H
#define TERMINATE_H

// Função responsável por destruir as texturas carregadas no jogo.
static void destroyTextures(Game *game) {
    // Verifica se as texturas existem e as destroem, liberando memória.
    if (game->player.texture) {
        SDL_DestroyTexture(game->player.texture);
    }
    if (game->dungeon.texture) {
        SDL_DestroyTexture(game->dungeon.texture);
    }
}

// Função responsável por encerrar o jogo e liberar todos os recursos alocados.
void terminate(Game *game) {    
    destroyTextures(game);

    // Destrói o renderizador e a janela, liberando memória.
    if (game->renderer) {
        SDL_DestroyRenderer(game->renderer);
    }
    if (game->window) {
        SDL_DestroyWindow(game->window);
    }

    // Finaliza a bibliotecas do SDL.
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    lua_close(game->L);
}

#endif // TERMINATE_H