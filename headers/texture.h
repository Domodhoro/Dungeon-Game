#ifndef TEXTURE_H
#define TEXTURE_H

// Função responsável por carregar as texturas do jogo.
static _Bool loadTexture(Game *game, const char *filePath, const TEXTURE_ID ID) {
    game->textures[ID] = IMG_LoadTexture(game->renderer, filePath);
    if (not game->textures[ID]) {
        SDL_Log("Falha ao carregar textura: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

// Função responsável por carregar as texturas dos objetos do jogo.
_Bool loadTextures(Game *game) {
    if (not loadTexture(game, "./assets/sprites/dungeon.png", DUNGEON_TEXTURE)) {
        return false;
    }
    if (not loadTexture(game, "./assets/sprites/player.png", PLAYER_TEXTURE)) {
        return false;
    }
    if (not loadTexture(game, "./assets/sprites/heart.png", HEART_TEXTURE)) {
        return false;
    }
    if (not loadTexture(game, "./assets/sprites/inventory.png", INVENTORY_TEXTURE)) {
        return false;
    }
    if (not loadTexture(game, "./assets/sprites/mainMenu.png", MAIN_MENU_TEXTURE)) {
        return false;
    }
    if (not loadTexture(game, "./assets/sprites/mainMenuBackground.png", MAIN_MENU_BACKGROUND_TEXTURE)) {
        return false;
    }
    return true;
}

// Função que destrói as texturas do jogo.
void destroyTextures(Game *game) {
    for (int i = 0; i < MAX_TEXTURES; i++) {
        if (game->textures[i]) {
            SDL_DestroyTexture(game->textures[i]);
            game->textures[i] = NULL;
        }
    }
}

#endif // TEXTURE_H