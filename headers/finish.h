#ifndef FINISH_H
#define FINISH_H

// Função responsável por encerrar o jogo e liberar todos os recursos alocados.
void finish(Game *game) {    
    // Verifica se os objetos do jogo existem e os destroem, liberando memória.
    destroyDungeon(game);
    destroyTextures(game);
    destroyFonts(game);

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
    if (game->lua) {
        lua_close(game->lua);
        game->lua = NULL;
    }
}

#endif // FINISH_H