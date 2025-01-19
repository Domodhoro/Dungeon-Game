#ifndef STRUCTS_H
#define STRUCTS_H

// Estrututa que representa um bloco.
typedef struct Block {
    SDL_Rect dst;
    SDL_Rect src;
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
    SDL_Rect dst;
    SDL_Rect src;
    SDL_Texture *texture;
} Entity;

// Estrututa que representa o inventário do jogador.
typedef struct Inventory {
    SDL_Rect dst[INVENTORY_SIZE];
    SDL_Rect src[INVENTORY_SIZE];
    SDL_Texture *texture;
} Inventory;

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
    lua_State *L;
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Rect viewport;
    SDL_Event event;

    Uint32 frameStart;    
    Uint32 frameTime;

    _Bool running;

    Keyboard keyboard;
    Mouse mouse;
    Camera camera;
    Inventory inventory;
    Entity *player;
    Dungeon *dungeon;
} Game;

#endif // STRUCTS_H