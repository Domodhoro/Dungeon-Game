#ifndef INIT_H
#define INIT_H

// Função responsável por iniciar um estado lua.
static _Bool initLua(Game *game) {
    game->lua = luaL_newstate();
    if (not game->lua) {
        fprintf(stderr, "Falha ao criar estado lua.\n");
        return false;
    }
    // Abre as bibliotecas do lua.
    luaL_openlibs(game->lua);

    // Abre/lê arquivo de script lua e caso ocorra erro, exibe uma mensagem e fecha o estado lua.
    if (luaL_dofile(game->lua, "./scripts/dungeon.lua") not_eq LUA_OK) {
        fprintf(stderr, "Falha ao abrir/ler arquivo de script lua: %s\n", lua_tostring(game->lua, -1));
        lua_close(game->lua);
        game->lua = NULL;
        return false;
    }
    return true;
}

// Função responsável por inicializar todos os recursos necessários para o jogo.
_Bool init(Game *game) {
    // Inicializa um estado lua.
    if (not initLua(game)) {
        return false;
    }

    // Inicializa a biblioteca para manipulação de vídeo.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Falha ao iniciar SDL: %s.\n", SDL_GetError());
        return false;
    }
    // Inicializa a biblioteca para carregar imagens.
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        fprintf(stderr, "Falha ao iniciar SDL_image: %s.\n", IMG_GetError());
        SDL_Quit();
        return false;
    }
    // Inicializa a biblioteca para carregar fontes.
    if (TTF_Init() < 0) {
        fprintf(stderr, "Falha ao iniciar SDL_ttf: %s.\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return false;
    }
    // Inicializa a biblioteca para manipulação de áudio.
    if (Mix_Init(MIX_INIT_MP3) < 0) {
        fprintf(stderr, "Falha ao inicializar SDL_mixer: %s.\n", Mix_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Cria a janela do jogo e o renderizador e caso ocorra erro, exibe uma mensagem.
    if (not createWindow(game) or not createRenderer(game)) {
        finish(game);
        return false;
    }

    // Carrega as textutas e as fontes do jogo.
    if (not loadTextures(game) or not loadFonts(game)) {
        finish(game);
        return false;
    }

    // Configura os objetos do jogo.
    if (not setup(game)) {
        finish(game);
        return false;
    }

    // Configura o estado do jogo para o menu principal.
    game->state = MAIN_MENU;
    return true;
}

#endif // INIT_H