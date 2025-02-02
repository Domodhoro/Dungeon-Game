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

// Função que destrói as salas e a masmorra.
static void destroyDungeon(Game *game) {
    if (game->dungeon) {
        // Começa pela primeira sala da masmorra.
        Room *currentRoom = game->dungeon->room;

        // Itera sobre todas as salas.
        while (currentRoom != NULL) {
            // Guarda o ponteiro para a próxima sala.
            Room *nextRoom = currentRoom->next;
            // Libera a memória da sala atual.
            free(currentRoom);                   
            // Avança para a próxima sala.
            currentRoom = nextRoom;              
        }

        // Após liberar todas as salas, zera o ponteiro para a lista de salas e libera a memória da masmorra.
        game->dungeon->room = NULL;
        free(game->dungeon);
        game->dungeon = NULL;
    }
}

// Função responsável por encerrar o jogo e liberar todos os recursos alocados.
void finish(Game *game) {    
    // Verifica se os objetos do jogo existem e os destroem, liberando memória.
    destroyTextures(game);
    destroyFonts(game);
    destroyDungeon(game);

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