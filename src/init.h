#ifndef INIT_H
#define INIT_H

// Inicializa o lua.
static BOOL initLua(Game *game) {
    // Inicializa um estado lua.
    game->L = luaL_newstate();
    if (NOT game->L) {
        fprintf(stderr, "Falha ao criar estado lua.\n");
        return FALSE;
    }
    // Abre as bibliotecas do lua.
    luaL_openlibs(game->L);
    // Abre/lê arquivo de script lua e caso ocorra erro, exibe uma mensagem.
    if (luaL_dofile(game->L, "./scripts/dungeon.lua") != LUA_OK) {
        fprintf(stderr, "Falha ao abrir/ler arquivo de script lua: %s\n", lua_tostring(game->L, -1));
        return FALSE;
    }
    return TRUE;
}

// Inicializa as bibliotecas do SDL.
static BOOL initSDL(Game *game) {
    // Inicializa a biblioteca para manipulação de vídeo.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Falha ao iniciar SDL: %s.\n", SDL_GetError());
        return FALSE;
    }
    // Inicializa a biblioteca para carregar imagens.
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        fprintf(stderr, "Falha ao iniciar SDL_image: %s.\n", IMG_GetError());
        SDL_Quit();
        return FALSE;
    }
    // Inicializa a biblioteca para carregar fontes.
    if (TTF_Init() < 0) {
        fprintf(stderr, "Falha ao iniciar SDL_ttf: %s.\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return FALSE;
    }
    // Inicializa a biblioteca para manipulação de áudio.
    if (Mix_Init(MIX_INIT_MP3) < 0) {
        fprintf(stderr, "Falha ao inicializar SDL_mixer: %s.\n", Mix_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return FALSE;
    }
    return TRUE;
}

// Função responsável por criar a janela do jogo.
static BOOL createWindow(Game *game) {
    // Cria a janela do jogo com o título e dimensões definidas e caso ocorra erro, exibe uma mensagem.
    game->window = SDL_CreateWindow(SCREEN_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (NOT game->window) {
        fprintf(stderr, "Falha ao criar janela de visualização: %s.\n", SDL_GetError());
        return FALSE;
    }

    // Posiciona o mouse no centro da janela.
    SDL_WarpMouseInWindow(game->window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    // Carrega a imagem do ícone da janela e caso ocorra erro ao carregar o ícone, exibe uma mensagem de erro.
    SDL_Surface *iconSurface = IMG_Load("./icon.png");
    if (NOT iconSurface) {
        fprintf(stderr, "Falha ao carregar o ícone: %s\n", IMG_GetError());
        return FALSE;
    }
    SDL_SetWindowIcon(game->window, iconSurface);
    // Libera a superfície que foi usada para o ícone.
    SDL_FreeSurface(iconSurface);
    return TRUE;
}

// Função responsável por criar o renderizador.
static BOOL createRenderer(Game *game) {
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (NOT game->renderer) {
        // Caso ocorra erro ao criar o renderizador, exibe a mensagem de erro.
        fprintf(stderr, "Falha ao criar o renderizador: %s.\n", SDL_GetError());
        return FALSE;
    }
    // Define as dimensões da janela de visualização do renderizador.
    game->viewport = (SDL_Rect) {
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
    };
    return TRUE;
}

// Função responsável por criar o jogador.
static BOOL createPlayer(Game *game) {
    // Aloca memória para a estrutura de dados do jogador.
    game->player = malloc(sizeof(Entity));
    if (NOT game->player) {
        fprintf(stderr, "Falha ao alocar mémoria para o jogador.\n");
        return FALSE;
    }
    // Zera a memória alocada para a estrutura da jogador.
    memset(game->player, 0, sizeof(Entity));

    // Define o retângulo de origem da textura do jogador.
    game->player->src = (SDL_Rect) {
        0, 0, 32, 32
    };
    // Define a posição e o tamanho do jogador na tela.
    game->player->dst = (SDL_Rect) {
        (SCREEN_WIDTH - PLAYER_WIDTH) / 2, (SCREEN_HEIGHT - PLAYER_HEIGHT) / 2, PLAYER_WIDTH, PLAYER_HEIGHT
    };

    // Carrega a textura do jogador e caso ocorra erro, exibe uma mensagem.
    game->player->texture = IMG_LoadTexture(game->renderer, "./img/player.png");
    if (NOT game->player->texture) {
        fprintf(stderr, "Falha ao carregar textura do jogador: %s\n", IMG_GetError());
        return FALSE;
    }
    return TRUE;
}

// Inicializa a câmera.
static void initCamera(Game *game) {
    // Define a posição inicial.
    game->camera.position = (SDL_Point) {
        600, 300
    };
    // Define a velocidade inicial.
    game->camera.speed = 5;
}

// Função responsável por inicializar todos os recursos necessários para o jogo.
BOOL init(Game *game) {
    // Abre um estado lua e inicializa o SDL.
    if (NOT initLua(game) OR NOT initSDL(game)) {
        return FALSE;
    }

    // Criação da janela do jogo e do renderizador.
    if (NOT createWindow(game) OR NOT createRenderer(game)) {
        finish(game);
        return FALSE;
    }

    // Criação do jogador e da masmorra.
    if (NOT createPlayer(game) OR NOT createDungeon(game)) {
        finish(game);
        return FALSE;
    }

    // Inicializa a posição da câmera.
    initCamera(game);
    return TRUE;
}

#endif // INIT_H