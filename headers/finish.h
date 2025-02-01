#ifndef FINISH_H
#define FINISH_H

// Função que destrói as texturas do jogo.
static void destroyTextures(Game *game) {
    Texture *current = game->texture;
    Texture *next = NULL;

    // Percorre toda a lista de texturas e destrói cada uma.
    while (current != NULL) {
        // Salva o próximo nó.
        next = current->next;

        // Destrói a textura atual.
        if (current->texture != NULL) {
            SDL_DestroyTexture(current->texture);
        }

        // Libera a memória associada à estrutura Texture.
        free(current);

        // Avança para o próximo nó.
        current = next;
    }

    for (int i = 0; i < 2; i++) {
        if (game->mainMenu.button[i].text.texture) {
            SDL_DestroyTexture(game->mainMenu.button[i].text.texture);
            game->mainMenu.button[i].text.texture = NULL;
        }
    }
}

// Função que destrói a fonte do jogo.
static void destroyFonts(Game *game) {
    if (game->font) {
        TTF_CloseFont(game->font);
        game->font = NULL;
    }
}

// Função responsável por encerrar o jogo e liberar todos os recursos alocados.
void finish(Game *game) {    
    // Verifica se os objetos do jogo existem e os destroem, liberando memória.
    destroyTextures(game);
    destroyFonts(game);

    /*
    // Destrói a masmorra, liberando memória.
    if (game->dungeon) {
        free(game->dungeon);
        game->dungeon = NULL;
    }
    */

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