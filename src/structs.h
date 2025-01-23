#ifndef STRUCTS_H
#define STRUCTS_H

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

// Estrutura para o menu principal do jogo.
typedef struct MainMenu {
    SDL_Rect src[3];
    SDL_Rect dst[3];
    SDL_Texture *texture;
    Text text[3];
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

#endif // STRUCTS_H