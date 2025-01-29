#ifndef INIT_H
#define INIT_H

// Função responsável por iniciar um estado lua.
static _Bool initLua(Game *game) {
    game->L = luaL_newstate();
    if (!game->L) {
        fprintf(stderr, "Falha ao criar estado lua.\n");
        return false;
    }
    // Abre as bibliotecas do lua.
    luaL_openlibs(game->L);
    // Abre/lê arquivo de script lua e caso ocorra erro, exibe uma mensagem.
    if (luaL_dofile(game->L, "./scripts/dungeon.lua") != LUA_OK) {
        fprintf(stderr, "Falha ao abrir/ler arquivo de script lua: %s\n", lua_tostring(game->L, -1));
        return false;
    }
    return true;
}

// Função responsável por iniciar as bibliotecas SDL.
static _Bool initSDL(Game *game) {
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

// Função que carrega as texturas do jogo.
static _Bool loadTextures(Game *game) {
    game->dungeon.texture            = IMG_LoadTexture(game->renderer, "./assets/sprites/dungeon.png");
    game->player.texture             = IMG_LoadTexture(game->renderer, "./assets/sprites/player.png");
    game->player.hearts.texture      = IMG_LoadTexture(game->renderer, "./assets/sprites/heart.png");
    game->inventory.texture          = IMG_LoadTexture(game->renderer, "./assets/sprites/inventory.png");
    game->mainMenu.texture           = IMG_LoadTexture(game->renderer, "./assets/sprites/mainMenu.png");
    game->mainMenu.backgroundTexture = IMG_LoadTexture(game->renderer, "./assets/sprites/mainMenuBackground.png");
    game->light.texture              = IMG_LoadTexture(game->renderer, "./assets/sprites/light.png");

    // Retorna falso em caso de erro.
    if (!game->dungeon.texture || !game->player.texture || !game->player.hearts.texture) {
        fprintf(stderr, "Falha ao carregar texturas da masmorra: %s\n", IMG_GetError());
        return false;
    }
    // Retorna falso em caso de erro.
    if (!game->inventory.texture || !game->mainMenu.texture || !game->mainMenu.backgroundTexture) {
        fprintf(stderr, "Falha ao carregar texturas da masmorra: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

// Função que carrega a fonte de texto.
static _Bool loadFonts(Game *game) {
    // Abre e carregar a fonte ttf.
    game->font[0] = TTF_OpenFont("./assets/fonts/04B_03__.TTF", FONT_SIZE);
    if (!game->font[0]) {
        fprintf(stderr, "Falha ao carregar a fonte: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

// Função responsável por inicializar todos os recursos necessários para o jogo.
_Bool init(Game *game) {
    // Inicializa um estado lua.
    if (!initLua(game)) {
        return false;
    }

    // Inicializa o SDL.
    if (!initSDL(game)) {
        return false;
    }

    // Cria a janela do jogo com o título e dimensões definidas e caso ocorra erro, exibe uma mensagem.
    game->window = SDL_CreateWindow(SCREEN_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!game->window) {
        fprintf(stderr, "Falha ao criar janela de visualização: %s.\n", SDL_GetError());
        finish(game);
        return false;
    }

    // Carrega a imagem do ícone da janela e caso ocorra erro ao carregar o ícone, exibe uma mensagem de erro.
    SDL_Surface *iconSurface = IMG_Load("./assets/sprites/icon.png");
    if (!iconSurface) {
        fprintf(stderr, "Falha ao carregar o ícone: %s\n", IMG_GetError());
        finish(game);
        return false;
    }
    SDL_SetWindowIcon(game->window, iconSurface);
    // Libera a superfície que foi usada para o ícone.
    SDL_FreeSurface(iconSurface);

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

    // Habilita o blending no renderizador.
    SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);

    // Define a cor de fundo.
    game->backgroundColor = BLACK;

    // Define as dimensões da janela de visualização do renderizador.
    game->viewport = (SDL_Rect) {
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
    };

    // Carrega as textutas e a fonte do jogo.
    if (!loadTextures(game) || !loadFonts(game)) {
        finish(game);
        return false;
    }

    // Configura os objetos do jogo.
    if (!setup(game)) {
        return false;
    }

    // Configura o estado do jogo para o menu principal.
    game->state = MAIN_MENU;
    return true;
}

#endif // INIT_H
