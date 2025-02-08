#ifndef INIT_H
#define INIT_H

// Função responsável por iniciar um estado lua.
static _Bool initLua(Game *game) {
    game->lua = luaL_newstate();
    if (!game->lua) {
        fprintf(stderr, "Falha ao criar estado lua.\n");
        return false;
    }
    // Abre as bibliotecas do lua.
    luaL_openlibs(game->lua);

    // Abre/lê arquivo de script lua e caso ocorra erro, exibe uma mensagem e fecha o estado lua.
    if (luaL_dofile(game->lua, "./scripts/dungeon.lua") != LUA_OK) {
        fprintf(stderr, "Falha ao abrir/ler arquivo de script lua: %s\n", lua_tostring(game->lua, -1));
        lua_close(game->lua);
        game->lua = NULL;
        return false;
    }
    return true;
}

// Função responsável por iniciar as bibliotecas SDL.
static _Bool initSDL() {
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
    return true;
}

// Função responsável por carregar as texturas dos objetos do jogo.
static _Bool loadTextures(Game *game) {
    if (!loadTexture(game, "./assets/sprites/dungeon.png", DUNGEON_TEXTURE)) {
        return false;
    }
    if (!loadTexture(game, "./assets/sprites/player.png", PLAYER_TEXTURE)) {
        return false;
    }
    if (!loadTexture(game, "./assets/sprites/heart.png", HEART_TEXTURE)) {
        return false;
    }
    if (!loadTexture(game, "./assets/sprites/inventory.png", INVENTORY_TEXTURE)) {
        return false;
    }
    if (!loadTexture(game, "./assets/sprites/mainMenu.png", MAIN_MENU_TEXTURE)) {
        return false;
    }
    if (!loadTexture(game, "./assets/sprites/mainMenuBackground.png", MAIN_MENU_BACKGROUND_TEXTURE)) {
        return false;
    }
    return true;
}

// Função responsável por carregar as fontes do jogo.
static _Bool loadFonts(Game *game) {
    if (!loadFont(game, "./assets/fonts/04B_03__.TTF", FONT_1, 24)) {
        return false;
    }
    return true;
}

// Função responsável por inicializar todos os recursos necessários para o jogo.
_Bool init(Game *game) {
    // Inicializa um estado lua e o SDL.
    if (!initLua(game) || !initSDL()) {
        return false;
    }

    // Cria a janela do jogo com o título e dimensões definidas e caso ocorra erro, exibe uma mensagem.
    game->window = SDL_CreateWindow(SCREEN_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!game->window) {
        fprintf(stderr, "Falha ao criar janela de visualização: %s.\n", SDL_GetError());
        finish(game);
        return false;
    }

    // Carrega e configura o ícone da janela de visualização e a textura do cursor do mouse.
    if (!loadWindowIcon(game, "./assets/sprites/icon.png") || !loadCursorTexture(game, "./assets/sprites/cursorTexture.png")) {
        finish(game);
        return false;
    }

    // Posiciona o mouse no centro da janela.
    SDL_WarpMouseInWindow(game->window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    // Criação e configuração do renderizador.
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!game->renderer) {
        // Caso ocorra erro ao criar o renderizador, exibe a mensagem de erro.
        fprintf(stderr, "Falha ao criar o renderizador: %s.\n", SDL_GetError());
        finish(game);
        return false;
    }

    // Define a cor de fundo.
    game->backgroundColor = GRAY;

    // Define as dimensões da janela de visualização do renderizador.
    game->viewport = (SDL_Rect) {
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
    };

    // Carrega as textutas e as fontes do jogo.
    if (!loadTextures(game) || !loadFonts(game)) {
        finish(game);
        return false;
    }

    // Configura os objetos do jogo.
    if (!setup(game)) {
        finish(game);
        return false;
    }

    // Configura o estado do jogo para o menu principal.
    game->state = MAIN_MENU;
    return true;
}

#endif // INIT_H