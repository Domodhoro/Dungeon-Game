// Bibliotecas padrão do C.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

// Bibliotecas SDL2.
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

// Bibliotecas lua (Linux e Windows).
#ifdef __linux__
    #include "./lua54/lua.h"
    #include "./lua54/lualib.h"
    #include "./lua54/lauxlib.h"
#elif _WIN32
    #include <lua/lua.h>
    #include <lua/lualib.h>
    #include <lua/lauxlib.h>
#endif

// Somente para a fase de desenvolvimento.
#define DEV true

// Define o título da janela do jogo, a largura e a altura da tela (em pixels).
#define SCREEN_TITLE "Dungeon (version 1.0)"
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

// Define a quantidade de quadros por segundo (FPS) desejada.
#define FPS 60
// Este valor é usado para controlar a taxa de atualização do jogo.
#define FRAME_TIME (1000 / FPS)

// Define o tamanho do jogador.
#define PLAYER_WIDTH 64
#define PLAYER_HEIGHT 64

// Define o tamanho dos blocos.
#define BLOCK_WIDTH 32
#define BLOCK_HEIGHT 32

// Define o tamanho máximo de uma masmorra.
#define DUNGEON_WIDTH 32
#define DUNGEON_HEIGHT 32

// Define o tamanho do inventário.
#define INVENTORY_SIZE 10

// Define o tamanho da fonte.
#define FONT_SIZE 24

// Define algumas constantes para cores.
#define WHITE (SDL_Color) {255, 255, 255}
#define BLACK (SDL_Color) {0, 0, 0}
#define RED (SDL_Color) {255, 0, 0}
#define GREEN (SDL_Color) {0, 255, 0}
#define BLUE (SDL_Color) {0, 0, 255}
#define YELLOW (SDL_Color) {255, 255, 0}
#define GRAY (SDL_Color) {50, 50, 50}
#define CYAN (SDL_Color) {0, 255, 255}
#define MAGENTA (SDL_Color) {255, 0, 255}
#define ORANGE (SDL_Color) {255, 165, 0}
#define PURPLE (SDL_Color) {128, 0, 128}
#define PINK (SDL_Color) {255, 182, 193}
#define BROWN (SDL_Color) {165, 42, 42}

// Estados do programa.
typedef enum GAME_STATE {
    MAIN_MENU = 0,
    ACTIVE,
    PAUSE
} GAME_STATE;

// Define as direções do jogador.
typedef enum DIRECTION {
    NONE = 0,
    UP,
    LEFT,
    DOWN,
    RIGHT
} DIRECTION;

// Define os tipos de blocos da masmorra.
typedef enum BLOCK_TYPE {
    AIR = 0,
    WOOD,
    STONE
} BLOCK_TYPE;

// Estrututa que representa um bloco.
typedef struct Block {
    SDL_Rect src;
    SDL_Rect dst;
    BLOCK_TYPE type;
    _Bool isSolid;
} Block;

// Estrututa que representa uma masmorra.
typedef struct Dungeon {
    Block block[DUNGEON_WIDTH][DUNGEON_HEIGHT];
    SDL_Texture *texture;
    int level;
} Dungeon;

// Estrutura que representa um objeto do jogo.
typedef struct Entity {
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Texture *texture;
} Entity;

// Estrututa que representa o inventário do jogador.
typedef struct Inventory {
    SDL_Rect src[INVENTORY_SIZE];
    SDL_Rect dst[INVENTORY_SIZE];
    SDL_Texture *texture;
} Inventory;

// Estrutura para texto.
typedef struct Text {
    SDL_Texture *texture;
    SDL_Rect dst;
} Text;

// Estrutura que representa um botão.
typedef struct Button {
    SDL_Rect src;
    SDL_Rect dst;
    Text text;
} Button;

// Estrutura para o menu principal do jogo.
typedef struct MainMenu {
    Button button[3];
    SDL_Texture *texture;
    SDL_Texture *backgroundTexture;
    SDL_Rect dst;
} MainMenu;

// Define a câmera do jogo.
typedef struct Camera {
    SDL_Point position;
    int speed;
} Camera;

// Estrutura que armazena os estados do mouse.
typedef struct Mouse {
    SDL_Point position;
    _Bool leftButton;
    _Bool rightButton;
} Mouse;

// Estrutura que armazena os estados das teclas do teclado.
typedef struct Keyboard {
    const Uint8 *states;
} Keyboard;

// Estrutura principal que representa os estados do jogo.
typedef struct Game {
    GAME_STATE state;

    lua_State *L;
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Color backgroundColor;
    SDL_Rect viewport;
    SDL_Event event;

    Uint32 frameStart;    
    Uint32 frameTime;

    _Bool running;

    TTF_Font *font;

    Keyboard keyboard;
    Mouse mouse;
    Camera camera;

    MainMenu *mainMenu;
    Inventory *inventory;
    Entity *player;
    Dungeon *dungeon;
} Game;

// Funções auxiliares.
_Bool init(Game *game);
void handleEvents(Game *game);
void update(Game *game);
void render(Game *game);
void finish(Game *game);
void renderMainMenu(Game *game);
void renderPlayer(Game *game);
void renderDungeon(Game *game);
void renderInventory(Game *game);
_Bool createWindow(Game *game);
_Bool createRenderer(Game *game);
_Bool createMainMenu(Game *game);
_Bool createPlayer(Game *game);
_Bool createDungeon(Game *game);
_Bool createInventory(Game *game);
void destroyTextures(Game *game);
void checkCollision(Game *game, const SDL_Point cameraPrev);
void updatePlayerTexture(Game *game, const DIRECTION direction);
void setBlockType(Game *game, const int i, const int j);
_Bool checkCursorArea(const SDL_Point cursorPosition, const Button *button);
Text createText(Game *game, const char *input, int x, int y, SDL_Color color);

// Função principal.
int main(int argc, char *argv[]) {
    // Aloca memória para a estrutura principal de dados do jogo.
    Game *game = malloc(sizeof(Game));
    if (!game) {
        fprintf(stderr, "Falha ao alocar mémoria para o jogo.\n");
        return -1;
    }
    // Zera a memória alocada para a estrutura principal.
    memset(game, 0, sizeof(Game));

    // Inicializa o jogo, se falhar, libera a memória e encerra o programa.
    game->running = init(game);
    if (!game->running) {
        free(game);
        game = NULL;
        return -1;
    }

    // Loop principal do jogo.
    while (game->running) {
        // Marca o início do frame atual.
        game->frameStart = SDL_GetTicks();
        // Processa os eventos, atualiza a lógica do jogo e renderiza a cena atual.
        handleEvents(game);
        update(game);
        render(game);
        // Calcula o tempo gasto no processamento do frame.
        game->frameTime = SDL_GetTicks() - game->frameStart;
        // Se o tempo do frame for menor que o tempo alvo (FRAME_TIME), espera o tempo restante.
        if (game->frameTime < (Uint32)FRAME_TIME) {
            SDL_Delay((Uint32)FRAME_TIME - game->frameTime);
        }
    }

    // Termina o jogo e libera recursos alocados.
    finish(game);
    // Libera a memória alocada para a estrutura do jogo.
    if (game) {
        free(game);
        game = NULL;
    }
    // Encerra o programa com sucesso.
    return 0;
}

// Função responsável por inicializar todos os recursos necessários para o jogo.
_Bool init(Game *game) {
    // Inicializa um estado lua.
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

    // Criação da janela do jogo e do renderizador.
    if (!createWindow(game) || !createRenderer(game)) {
        finish(game);
        return false;
    }

    // Abre e carregar a fonte ttf.
    game->font = TTF_OpenFont("./font/04B_03__.TTF", FONT_SIZE);
    if (!game->font) {
        fprintf(stderr, "Falha ao carregar a fonte: %s\n", TTF_GetError());
        finish(game);
        return false;
    }

    // Criação dos objetos do jogo.
    if (!createMainMenu(game) || !createPlayer(game) || !createDungeon(game) || !createInventory(game)) {
        finish(game);
        return false;
    }

    // Define a posição inicial da câmera.
    game->camera.position = (SDL_Point) {
        0, 0
    };
    // Define a velocidade inicial.
    game->camera.speed = 5;

    // Configura o estado do jogo para o menu principal.
    game->state = MAIN_MENU;
    return true;
}

// Função que trata os eventos.
void handleEvents(Game *game) {
    // Verifica se há eventos na fila de eventos do SDL.
    while (SDL_PollEvent(&game->event)) {
        switch (game->event.type) {
#if DEV
        // Processa o evento tecla ESC pressionada (somente para a fase de desenvolvimento).
        case SDL_KEYDOWN:
            if (game->event.key.keysym.sym == SDLK_ESCAPE) {
                // Se a tecla pressionada for ESC, encerra o jogo.
                game->running = false;
            }
            break;
#endif // DEV
        // Se o evento for de fechar a janela, o jogo termina.
        case SDL_QUIT:
            game->running = false;
            break;
        // Verifica eventos de clique do mouse.
        case SDL_MOUSEBUTTONDOWN:
            if (game->event.button.button == SDL_BUTTON_LEFT) {
                // Botão esquerdo pressionado.
                game->mouse.leftButton = true;
            } else if (game->event.button.button == SDL_BUTTON_RIGHT) {
                // Botão direito pressionado.
                game->mouse.rightButton = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (game->event.button.button == SDL_BUTTON_LEFT) {
                // Botão esquerdo solto.
                game->mouse.leftButton = false;
            } else if (game->event.button.button == SDL_BUTTON_RIGHT) {
                // Botão direito solto.
                game->mouse.rightButton = false;
            }
            break;
        }
    }

    // Retorna as coordenadas atuais do cursor do mouse.
    SDL_GetMouseState(&game->mouse.position.x, &game->mouse.position.y);

    // Atualiza o estado atual do teclado.
    game->keyboard.states = SDL_GetKeyboardState(NULL);
}

// Função responsável por atualizar o estado do jogo.
void update(Game *game) {
    DIRECTION direction = NONE;

    // Armazena a posição da câmera antes do movimento.
    SDL_Point cameraPrev = game->camera.position;

    // Move a câmera para cima.
    if (game->keyboard.states[SDL_SCANCODE_W]) {
        direction = UP;
        game->camera.position.y += game->camera.speed;
    }
    // Move a câmera para a esquerda.
    if (game->keyboard.states[SDL_SCANCODE_A]) {
        direction = LEFT;
        game->camera.position.x += game->camera.speed;
    }
    // Move a câmera para baixo.
    if (game->keyboard.states[SDL_SCANCODE_S]) {
        direction = DOWN;
        game->camera.position.y -= game->camera.speed;
    }
    // Move a câmera para a direita.
    if (game->keyboard.states[SDL_SCANCODE_D]) {
        direction = RIGHT;
        game->camera.position.x -= game->camera.speed;
    }

    // Verifica se há colisão antes de atualizar os blocos.
    checkCollision(game, cameraPrev);

    // Atualiza a posição dos blocos somente se não houver colisão.
    for (int i = 0; i < DUNGEON_WIDTH; i++) {
        for (int j = 0; j < DUNGEON_HEIGHT; j++) {
            game->dungeon->block[i][j].dst.x = i * BLOCK_WIDTH + game->camera.position.x;
            game->dungeon->block[i][j].dst.y = j * BLOCK_HEIGHT + game->camera.position.y;
        }
    }

    // Atualiza a textura do jogador.
    updatePlayerTexture(game, direction);

    // Verifica qual dos botões do menu principal foi clicado.
    for (int i = 0; i < 3; i++) {
        if (checkCursorArea(game->mouse.position, &game->mainMenu->button[i])) {
            game->mainMenu->button[i].src.y = 32;
            if (game->mouse.leftButton) {
                if (i == 0) {
                    game->state = ACTIVE;
                } else if (i == 1) {

                } else if (i == 2) {
                    game->running = false;
                }
            }
        } else {
            game->mainMenu->button[i].src.y = 0;
        }
    }
}

// Função responsável por renderizar o conteúdo na tela.
void render(Game *game) {
    // Define a cor de fundo da tela.
    SDL_SetRenderDrawColor(game->renderer, game->backgroundColor.r, game->backgroundColor.g, game->backgroundColor.b, 255);
    SDL_RenderClear(game->renderer);

    // Renderiza os elementos do jogo com base no estado do mesmo.
    switch (game->state) {
    case MAIN_MENU:
        renderMainMenu(game);
        break;
    case ACTIVE:
        renderDungeon(game);
        renderInventory(game);
        renderPlayer(game);
        break;
    }
    
    // Apresenta o conteúdo renderizado na tela.
    SDL_RenderSetViewport(game->renderer, &game->viewport);
    SDL_RenderPresent(game->renderer);
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

// Renderiza o menu principal.
void renderMainMenu(Game *game) {
    // Renderiza o fundo do menu principal.
    SDL_RenderCopy(game->renderer, game->mainMenu->backgroundTexture, NULL, &game->mainMenu->dst);
    // Renderiza os botões do menu principal.
    for (int i = 0; i < 3; i++) {
        SDL_RenderCopy(game->renderer, game->mainMenu->texture, &game->mainMenu->button[i].src, &game->mainMenu->button[i].dst); 
        SDL_RenderCopy(game->renderer, game->mainMenu->button[i].text.texture, NULL, &game->mainMenu->button[i].text.dst);  
    }
}

// Renderiza o jogador.
void renderPlayer(Game *game) {
    SDL_RenderCopy(game->renderer, game->player->texture, &game->player->src, &game->player->dst);
}

// Renderiza a masmorra.
void renderDungeon(Game *game) {
    for (int i = 0; i < DUNGEON_WIDTH; i++) {
        for (int j = 0; j < DUNGEON_HEIGHT; j++) {
            if (game->dungeon->block[i][j].type != AIR) {
                // Renderiza os blocos da masmorra.
                SDL_RenderCopy(game->renderer, game->dungeon->texture, &game->dungeon->block[i][j].src, &game->dungeon->block[i][j].dst);
            }
        }
    }
}

// Renderiza o inventário do jogador.
void renderInventory(Game *game) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        SDL_RenderCopy(game->renderer, game->inventory->texture, &game->inventory->src[i], &game->inventory->dst[i]);
    }
}

// Função responsável por criar a janela do jogo.
_Bool createWindow(Game *game) {
    // Cria a janela do jogo com o título e dimensões definidas e caso ocorra erro, exibe uma mensagem.
    game->window = SDL_CreateWindow(SCREEN_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!game->window) {
        fprintf(stderr, "Falha ao criar janela de visualização: %s.\n", SDL_GetError());
        return false;
    }

    // Carrega a imagem do ícone da janela e caso ocorra erro ao carregar o ícone, exibe uma mensagem de erro.
    SDL_Surface *iconSurface = IMG_Load("./icon.png");
    if (!iconSurface) {
        fprintf(stderr, "Falha ao carregar o ícone: %s\n", IMG_GetError());
        return false;
    }
    SDL_SetWindowIcon(game->window, iconSurface);
    // Libera a superfície que foi usada para o ícone.
    SDL_FreeSurface(iconSurface);

    // Posiciona o mouse no centro da janela.
    SDL_WarpMouseInWindow(game->window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    return true;
}

// Função responsável por criar o renderizador.
_Bool createRenderer(Game *game) {
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!game->renderer) {
        // Caso ocorra erro ao criar o renderizador, exibe a mensagem de erro.
        fprintf(stderr, "Falha ao criar o renderizador: %s.\n", SDL_GetError());
        return false;
    }

    // Define a cor de fundo.
    game->backgroundColor = GRAY;

    // Define as dimensões da janela de visualização do renderizador.
    game->viewport = (SDL_Rect) {
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
    };
    return true;
}

// Função responsável por criar o menu principal do jogo.
_Bool createMainMenu(Game *game) {
    // Aloca memória para a estrutura do menu principal.
    game->mainMenu = malloc(sizeof(MainMenu));
    if (!game->mainMenu) {
        fprintf(stderr, "Falha ao alocar mémoria para o menu principal.\n");
        return false;
    }
    // Zera a memória alocada para a estrutura da jogador.
    memset(game->mainMenu, 0, sizeof(MainMenu));

    // Carrega a textura do menu principal e caso ocorra erro, exibe uma mensagem.
    game->mainMenu->texture = IMG_LoadTexture(game->renderer, "./img/mainMenu.png");
    if (!game->mainMenu->texture) {
        fprintf(stderr, "Falha ao carregar textura do menu principal: %s\n", IMG_GetError());
        return false;
    }

    // Carrega a textura do fundo menu principal e caso ocorra erro, exibe uma mensagem.
    game->mainMenu->backgroundTexture = IMG_LoadTexture(game->renderer, "./img/mainMenuBackground.png");
    if (!game->mainMenu->backgroundTexture) {
        fprintf(stderr, "Falha ao carregar textura do fundo do menu principal: %s\n", IMG_GetError());
        return false;
    }

    // Define a textura dos botões e o posicionamento na tela.
    const int buttonWidth = 5 * 32;
    const int buttonHeight = 32;
    // Botão 'start'.
    game->mainMenu->button[0].src = (SDL_Rect) {
        0, 0, buttonWidth, buttonHeight
    };
    game->mainMenu->button[0].dst = (SDL_Rect) {
        (SCREEN_WIDTH - buttonWidth) / 2, (SCREEN_HEIGHT - buttonHeight) / 2 - 250, buttonWidth, buttonHeight
    };
    // Botão 'settings'.
    game->mainMenu->button[1].src = (SDL_Rect) {
        0, 0, buttonWidth, buttonHeight
    };
    game->mainMenu->button[1].dst = (SDL_Rect) {
        (SCREEN_WIDTH - buttonWidth) / 2, (SCREEN_HEIGHT - buttonHeight) / 2 - 200, buttonWidth, buttonHeight
    };
    // Botão 'exit'.
    game->mainMenu->button[2].src = (SDL_Rect) {
        0, 0, buttonWidth, buttonHeight
    };
    game->mainMenu->button[2].dst = (SDL_Rect) {
        (SCREEN_WIDTH - buttonWidth) / 2, (SCREEN_HEIGHT - buttonHeight) / 2 - 150, buttonWidth, buttonHeight
    };

    // Textos para o menu principal.
    game->mainMenu->button[0].text = createText(game, "Start", 0, 0, BLACK);
    game->mainMenu->button[1].text = createText(game, "Settings", 0, 0, BLACK);
    game->mainMenu->button[2].text = createText(game, "Exit", 0, 0, BLACK);
    if (!game->mainMenu->button[0].text.texture || !game->mainMenu->button[1].text.texture || !game->mainMenu->button[2].text.texture) {
        return false;
    }
    // Posiciona os textos no centro das texturas dos botões.
    game->mainMenu->button[0].text.dst.x = (SCREEN_WIDTH - game->mainMenu->button[0].text.dst.w) / 2;
    game->mainMenu->button[0].text.dst.y = (SCREEN_HEIGHT - game->mainMenu->button[0].text.dst.h) / 2 - 250;
    game->mainMenu->button[1].text.dst.x = (SCREEN_WIDTH - game->mainMenu->button[1].text.dst.w) / 2;
    game->mainMenu->button[1].text.dst.y = (SCREEN_HEIGHT - game->mainMenu->button[1].text.dst.h) / 2 - 200;
    game->mainMenu->button[2].text.dst.x = (SCREEN_WIDTH - game->mainMenu->button[2].text.dst.w) / 2;
    game->mainMenu->button[2].text.dst.y = (SCREEN_HEIGHT - game->mainMenu->button[2].text.dst.h) / 2 - 150;

    // Define o posicionamento da textura de fundo na tela.
    game->mainMenu->dst = (SDL_Rect) {
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
    };
    return true;
}

// Função responsável por criar o jogador.
_Bool createPlayer(Game *game) {
    // Aloca memória para a estrutura de dados do jogador.
    game->player = malloc(sizeof(Entity));
    if (!game->player) {
        fprintf(stderr, "Falha ao alocar mémoria para o jogador.\n");
        return false;
    }
    // Zera a memória alocada para a estrutura da jogador.
    memset(game->player, 0, sizeof(Entity));

    // Carrega a textura do jogador e caso ocorra erro, exibe uma mensagem.
    game->player->texture = IMG_LoadTexture(game->renderer, "./img/player.png");
    if (!game->player->texture) {
        fprintf(stderr, "Falha ao carregar textura do jogador: %s\n", IMG_GetError());
        return false;
    }

    // Define o retângulo de origem da textura do jogador.
    game->player->src = (SDL_Rect) {
        0, 0, 32, 32
    };
    // Define a posição e o tamanho do jogador na tela.
    game->player->dst = (SDL_Rect) {
        (SCREEN_WIDTH - PLAYER_WIDTH) / 2, (SCREEN_HEIGHT - PLAYER_HEIGHT) / 2, PLAYER_WIDTH, PLAYER_HEIGHT
    };
    return true;
}

// Função responsável por criar uma masmorra.
_Bool createDungeon(Game *game) {
    // Aloca memória para a estrutura de dados da masmorra.
    game->dungeon = malloc(sizeof(Dungeon));
    if (!game->dungeon) {
        fprintf(stderr, "Falha ao alocar memória para a masmorra.\n");
        return false;
    }
    // Zera a memória alocada para a estrutura da masmorra.
    memset(game->dungeon, 0, sizeof(Dungeon));

    // Carrega a textura da masmorra.
    game->dungeon->texture = IMG_LoadTexture(game->renderer, "./img/dungeon.png");
    if (!game->dungeon->texture) {
        fprintf(stderr, "Falha ao carregar texturas da masmorra: %s\n", IMG_GetError());
        return false;
    }

    // Acessa a tabela principal.
    lua_getglobal(game->L, "dungeon");
    if (!lua_istable(game->L, -1)) {
        fprintf(stderr, "Erro ao acessar a tabela 'dungeon': %s\n", lua_tostring(game->L, -1));
        lua_pop(game->L, 1);
        return false;
    }

    // Acessa a sala específica usando o nome fornecido.
    const char *roomName = "room_3";
    lua_getfield(game->L, -1, roomName);
    if (!lua_istable(game->L, -1)) {
        fprintf(stderr, "Erro ao acessar a sala '%s': %s\n", roomName, lua_tostring(game->L, -1));
         // Remove a tabela principal e o erro da pilha.
        lua_pop(game->L, 2);
        return false;
    }

    // Preenche a masmorra com blocos a partir dos dados da tabela lua.
    for (int i = 0; i < DUNGEON_WIDTH; i++) {
        for (int j = 0; j < DUNGEON_HEIGHT; j++) {
            // Calcula o índice na tabela.
            lua_pushnumber(game->L, i * DUNGEON_HEIGHT + j + 1);
            lua_gettable(game->L, -2);

            // Verifica se o valor é um número.
            if (lua_isnumber(game->L, -1)) {
                // Define as propriedades de cada bloco.
                setBlockType(game, i, j);
            } else {
                // Em caso de erro na leitura da tabela define o bloco como ar.
                game->dungeon->block[i][j].type = AIR;
                game->dungeon->block[i][j].isSolid = false;
            }
            // Remove o valor da pilha.
            lua_pop(game->L, 1);

            // COnfigura a posição dos blocos.
            game->dungeon->block[i][j].dst = (SDL_Rect) { 
                i * BLOCK_WIDTH, j * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT 
            };
        }
    }

    // Lê o nível da masmorra.
    lua_getfield(game->L, -1, "level");
    if (lua_isnumber(game->L, -1)) {
        game->dungeon->level = (int)lua_tonumber(game->L, -1);
    } else {
        game->dungeon->level = 1;
    }
    // Remove a referência à sala.
    lua_pop(game->L, 1);
    return true;
}

// Função responsável por criar o inventário do jogador.
_Bool createInventory(Game *game) {
    // Aloca memória para a estrutura de dados do inventário.
    game->inventory = malloc(sizeof(Dungeon));
    if (!game->inventory) {
        fprintf(stderr, "Falha ao alocar memória para o inventário.\n");
        return false;
    }
    // Zera a memória alocada para a estrutura do inventário.
    memset(game->inventory, 0, sizeof(Inventory));

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        game->inventory->src[i] = (SDL_Rect) {
            0, 0, 32, 32
        };
        game->inventory->dst[i] = (SDL_Rect) {
            i * 32 + (SCREEN_WIDTH - 320) / 2, SCREEN_HEIGHT - 32, 32, 32
        };
    }

    // Carrega a textura do inventário e caso ocorra erro, exibe uma mensagem.
    game->inventory->texture = IMG_LoadTexture(game->renderer, "./img/inventory.png");
    if (!game->inventory->texture) {
        fprintf(stderr, "Falha ao carregar textura do inventário: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

// Função responsável por destruir as texturas carregadas no jogo.
void destroyTextures(Game *game) {
    // Verifica se as texturas existem e as destroem, liberando memória.
    if (game->mainMenu->texture) {
        SDL_DestroyTexture(game->mainMenu->texture);
        game->mainMenu->texture = NULL;
    }
    if (game->mainMenu->backgroundTexture) {
        SDL_DestroyTexture(game->mainMenu->backgroundTexture);
        game->mainMenu->backgroundTexture = NULL;
    }
    for (int i = 0; i < 3; i++) {
        if (game->mainMenu->button[i].text.texture) {
            SDL_DestroyTexture(game->mainMenu->button[i].text.texture);
            game->mainMenu->button[i].text.texture = NULL;
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

// Verifica colisões.
void checkCollision(Game *game, const SDL_Point cameraPrev) {
    SDL_Rect blockDst;
    for (int i = 0; i < DUNGEON_WIDTH; i++) {
        for (int j = 0; j < DUNGEON_HEIGHT; j++) {
            if (game->dungeon->block[i][j].isSolid) {
                blockDst = game->dungeon->block[i][j].dst;

                blockDst.x = i * BLOCK_WIDTH + game->camera.position.x;
                blockDst.y = j * BLOCK_HEIGHT + game->camera.position.y;

                if (SDL_HasIntersection(&game->player->dst, &blockDst)) {
                    // Se houver colisão, desfaz o movimento antes de atualizar os blocos.
                    game->camera.position = cameraPrev;
                    // Sai do loop assim que detectar a colisão.
                    break;
                }
            }
        }
    }
}

// Atualiza a textura do jogador com base na direção.
void updatePlayerTexture(Game *game, const DIRECTION direction) {
    switch ((int)direction) {
        case UP:
            game->player->src.x = 0;
            game->player->src.y = 0;
            break;
        case DOWN:
            game->player->src.x = 32;
            game->player->src.y = 0;
            break;
        case RIGHT:
            game->player->src.x = 32;
            game->player->src.y = 0;
            break;
        case LEFT:
            game->player->src.x = 0;
            game->player->src.y = 0;
            break;
    }
}

// Define o tipo de bloco.
void setBlockType(Game *game, const int i, const int j) {
    // Lê o tipo de bloco da tabela lua.
    game->dungeon->block[i][j].type = (int)lua_tonumber(game->L, -1);
    // Define as propriedades de cada bloco.
    switch (game->dungeon->block[i][j].type) {
    case AIR:
        game->dungeon->block[i][j].isSolid = false;
        break;
    case WOOD:
        game->dungeon->block[i][j].src = (SDL_Rect) { 
            0, 0, 32, 32 
        };
        game->dungeon->block[i][j].isSolid = false;
        break;
    case STONE:
        game->dungeon->block[i][j].src = (SDL_Rect) { 
            32, 0, 32, 32 
        };
        game->dungeon->block[i][j].isSolid = true;
        break;
    }
}

// Função que verifica se o cursor do mouse está dentro da área dos botões do menu principal.
_Bool checkCursorArea(const SDL_Point cursorPosition, const Button *button) {
    if (cursorPosition.x >= button->dst.x && cursorPosition.x <= button->dst.x + button->dst.w) {
        if (cursorPosition.y >= button->dst.y && cursorPosition.y <= button->dst.y + button->dst.h) {
            return true;
        }
    }
    return false;
}

// Criar a superfície com o texto.
Text createText(Game *game, const char *input, int x, int y, SDL_Color color) {
    // Cria uma estrutura de texto.
    Text text = {
        NULL,
        (SDL_Rect) {0, 0, 0, 0}
    };

    SDL_Surface *textSurface = TTF_RenderText_Blended(game->font, input, color);
    if (!textSurface) {
        printf("Erro ao criar superfície de texto: %s\n", TTF_GetError());
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