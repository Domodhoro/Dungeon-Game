#ifndef TEXT_H
#define TEXT_H

// Função que carrega a fonte de texto.
static _Bool loadFont(Game *game, const char *filePath, const FONT_ID ID, const int fontSize) {
    game->font[ID] = TTF_OpenFont(filePath, fontSize);
    if (not game->font[ID]) {
        SDL_Log("Falha ao carregar a fonte: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

// Função responsável por carregar as fontes do jogo.
_Bool loadFonts(Game *game) {
    if (not loadFont(game, "./assets/fonts/04B_03__.TTF", FONT_1, 24)) {
        return false;
    }
    return true;
}

// Criar a superfície com o texto.
Text newText(Game *game, const char *input, int x, int y, SDL_Color color) {
    // Cria uma estrutura de texto.
    Text text;
    SDL_Surface *textSurface = TTF_RenderText_Blended(game->font[0], input, color);
    if (not textSurface) {
        SDL_Log("Erro ao criar superfície de texto: %s\n", TTF_GetError());
        return text;
    }

    // Definir o retângulo para renderizar o texto.
    text.dst = (SDL_Rect) {
        x, y, textSurface->w, textSurface->h
    };

    // Criar a textura a partir da superfície.
    text.texture = SDL_CreateTextureFromSurface(game->renderer, textSurface);
    // Liberar a superfície após criar a textura.
    SDL_FreeSurface(textSurface);
    return text;
}

// Função que destrói a fonte do jogo.
void destroyFonts(Game *game) {
    for (int i = 0; i < MAX_FONTS; i++) {
        if (game->font[i]) {
            TTF_CloseFont(game->font[i]);
            game->font[i] = NULL;
        }
    }
}

#endif // TEXT_H