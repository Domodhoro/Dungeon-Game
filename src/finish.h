#ifndef FINISH_H
#define FINISH_H

// Função responsável por destruir as texturas carregadas no jogo.
static void destroyTextures(Game *game) {
    // Verifica se as texturas existem e as destroem, liberando memória.
    if (game->mainMenu->texture) {
        SDL_DestroyTexture(game->mainMenu->texture);
        game->mainMenu->texture = NULL;
    }
    for (int i = 0; i < 3; i++) {
        if (game->mainMenu->text[i].texture) {
            SDL_DestroyTexture(game->mainMenu->text[i].texture);
            game->mainMenu->text[i].texture = NULL;
        }
    }
    if (game->player->texture) {
        SDL_DestroyTexture(game->player->texture);
        game->player->texture = NULL;
    }
    if (game->dungeon->texture) {
        SDL_DestroyTexture(game->dungeon->texture);
        game->dungeon->texture = NULL;
    }
    if (game->inventory->texture) {
        SDL_DestroyTexture(game->inventory->texture);
        game->inventory->texture = NULL;
    }
}

// Função responsável por encerrar o jogo e liberar todos os recursos alocados.
void finish(Game *game) {    
    destroyTextures(game);

    // Destroy a fonte.
    if (game->font) {
        TTF_CloseFont(game->font);
        game->font = NULL;
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

    // Destrói o menu principal, liberando memória.
    if (game->mainMenu) {
        free(game->mainMenu);
        game->mainMenu = NULL;
    }
    // Destrói o inventário, liberando memória.
    if (game->inventory) {
        free(game->inventory);
        game->inventory = NULL;
    }
    // Destrói a masmorra, liberando memória.
    if (game->dungeon) {
        free(game->dungeon);
        game->dungeon = NULL;
    }
    // Destrói o jogador, liberando memória.
    if (game->player) {
        free(game->player);
        game->player = NULL;
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