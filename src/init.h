#ifndef INIT_H
#define INIT_H

static _Bool initLua(Game *game) {
    // Inicializa um estado lua.
    game->L = luaL_newstate();
    if (!game->L) {
        fprintf(stderr, "Falha ao criar estado lua.\n");
        return false;
    }
    // Abre as bibliotecas do lua.
    luaL_openlibs(game->L);
    // Abre/lê arquivo de script lua e caso ocorra erro, exibe uma mensagem.
    if (luaL_dofile(game->L, "./scripts/script.lua") != LUA_OK) {
        fprintf(stderr, "Falha ao abrir/ler arquivo de script lua: %s\n", lua_tostring(game->L, -1));
        return false;
    }
    return true;
}

// Função responsável por criar a janela do jogo.
static _Bool createWindow(Game *game) {
    // Cria a janela do jogo com o título e dimensões definidas e caso ocorra erro, exibe uma mensagem.
    game->window = SDL_CreateWindow(SCREEN_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!game->window) {
        fprintf(stderr, "Falha ao criar janela de visualização: %s.\n", SDL_GetError());
        return false;
    }

    // Posiciona o mouse no centro da janela.
    SDL_WarpMouseInWindow(game->window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    // Carrega a imagem do ícone da janela e caso ocorra erro ao carregar o ícone, exibe uma mensagem de erro.
    SDL_Surface *iconSurface = IMG_Load("./icon.png");
    if (!iconSurface) {
        fprintf(stderr, "Falha ao carregar o ícone: %s\n", IMG_GetError());
        return false;
    }
    SDL_SetWindowIcon(game->window, iconSurface);
    // Libera a superfície que foi usada para o ícone.
    SDL_FreeSurface(iconSurface);
    return true;
}

// Função responsável por criar o renderizador.
static _Bool createRenderer(Game *game) {
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!game->renderer) {
        // Caso ocorra erro ao criar o renderizador, exibe a mensagem de erro.
        fprintf(stderr, "Falha ao criar o renderizador: %s.\n", SDL_GetError());
        return false;
    }
    return true;
}

// Função responsável por criar o jogador.
static _Bool createPlayer(Game *game) {
    // Define as dimensões do jogador em pixels.
    const int playerWidth = 64;
    const int playerheight = 64;
    // Define o retângulo de origem da textura do jogador.
    game->player.src = (SDL_Rect) {
        0, 0, 32, 32
    };
    // Define a posição e o tamanho do jogador na tela.
    game->player.dst = (SDL_Rect) {
        (SCREEN_WIDTH - playerWidth) / 2, (SCREEN_HEIGHT - playerheight) / 2, playerWidth, playerheight
    };
    // Define a velocidade do jogador.
    game->player.speed = 3;
    // Carrega a textura do jogador e caso ocorra erro, exibe uma mensagem.
    game->player.texture = IMG_LoadTexture(game->renderer, "./img/player.png");
    if (!game->player.texture) {
        fprintf(stderr, "Falha ao carregar textura do jogador: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

// Função responsável por criar uma masmorra.
static _Bool createDungeon(Game *game) {
    // Acessa a tabela na pilha lua.
    lua_getglobal(game->L, "dungeon");

    // Verifica se é uma tabela.
    if (!lua_istable(game->L, -1)) {
        fprintf(stderr, "Erro ao ler a tabela 'dungeon': %s\n", lua_tostring(game->L, -1));
        lua_pop(game->L, 1);
        return false;
    }

    int i = 0;
    int j = 0;
    for (i = 0; i != DUNGEON_WIDTH; i++) {
        for (j = 0; j != DUNGEON_HEIGHT; j++) {
            // Calcula o índice na tabela.
            int index = i * DUNGEON_HEIGHT + j + 1;
            // Coloca o índice na pilha para pegar o valor correspondente.
            lua_pushnumber(game->L, index);
            lua_gettable(game->L, -2);
            // Verifica se o valor é um número.
            if (lua_isnumber(game->L, -1)) {
                game->dungeon.data[i][j] = (int)lua_tonumber(game->L, -1);
            } else {
                // Caso não seja um número, define um valor padrão.
                game->dungeon.data[i][j] = AIR;
            }
            // Remove o valor da pilha.
            lua_pop(game->L, 1);
        }
    }
    game->dungeon.dst = (SDL_Rect) {
        0, 0, 32, 32
    };
    game->dungeon.src = (SDL_Rect) {
        0, 0, 32, 32
    };
    // Carrega a textura dos blocos da masmorra e caso ocorra erro, exibe uma mensagem.
    game->dungeon.texture = IMG_LoadTexture(game->renderer, "./img/dungeon.png");
    if (!game->dungeon.texture) {
        fprintf(stderr, "Falha ao carregar textura da masmorra: %s\n", IMG_GetError());
        return false;
    }
    // Lê o nível da masmorra.
    lua_getfield(game->L, -1, "level");
    if (lua_isnumber(game->L, -1)) {
        game->dungeon.level = (int)lua_tonumber(game->L, -1);
    } else {
        game->dungeon.level = 1;
    }
    // Remove a tabela da pilha.
    lua_pop(game->L, 1);
    return true;
}

// Função responsável por inicializar todos os recursos necessários para o jogo.
_Bool init(Game *game) {
    // Abre um estado lua.
    if (!initLua(game)) {
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

    // Criação da janela do jogo.
    if (!createWindow(game)) {
        terminate(game);
        return false;
    }
    // Criação do renderizador.
    if (!createRenderer(game)) {
        terminate(game);
        return false;
    }
    // Criação do jogador.
    if (!createPlayer(game)) {
        terminate(game);
        return false;
    }
    // Criação da masmorra.
    if (!createDungeon(game)) {
        terminate(game);
        return false;
    }

    // Inicializa a posição da câmera.
    game->camera.position = (SDL_Point) {
        (SCREEN_WIDTH - 320) / 2, (SCREEN_HEIGHT - 320) / 2
    };

    return true;
}

#endif // INIT_H