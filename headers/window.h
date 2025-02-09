#ifndef WINDOW_H
#define WINDOW_H

// Função responsável por configurar o ícone da janela de visualização do jogo.
static _Bool loadWindowIcon(Game *game, const char *filePath) {
    // Carrega a imagem do ícone da janela e caso ocorra erro ao carregar o ícone, exibe uma mensagem de erro.
    SDL_Surface *iconSurface = IMG_Load(filePath);
    if (not iconSurface) {
        SDL_Log("Falha ao carregar o ícone da janela de visualização: %s\n", IMG_GetError());
        return false;
    }
    SDL_SetWindowIcon(game->window, iconSurface);
    // Libera a superfície que foi usada para o ícone.
    SDL_FreeSurface(iconSurface);
    iconSurface = NULL;
    return true;
}

// Cria a janela do jogo com o título e dimensões definidas e caso ocorra erro, exibe uma mensagem.
_Bool createWindow(Game *game) {
    game->window = SDL_CreateWindow(SCREEN_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (not game->window) {
        SDL_Log("Falha ao criar janela de visualização: %s.\n", SDL_GetError());
        return false;
    }

    // Carrega e configura o ícone da janela de visualização e a textura do cursor do mouse.
    if (not loadWindowIcon(game, "./assets/sprites/icon.png")) {
        return false;
    }

    // Posiciona o mouse no centro da janela.
    SDL_WarpMouseInWindow(game->window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    return true;
}

#endif // WINDOW_H