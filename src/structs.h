#ifndef STRUCTS_H
#define STRUCTS_H

// Estrutura que representa uma masmorra do jogo.
typedef struct Dungeon {
    SDL_Rect dst;
    SDL_Rect src;
    int level;
    int data[DUNGEON_WIDTH][DUNGEON_WIDTH];
    SDL_Texture *texture;
} Dungeon;

// Estrutura que representa uma entidade do jogo com propriedades de renderização e movimento.
typedef struct Entity {
    SDL_Rect dst;
    SDL_Rect src;
    int speed;
    SDL_Texture *texture;
} Entity;

// Define a câmera do jogo.
typedef struct Camera {
    SDL_Point position;
} Camera;

// Estrutura que armazena os estados do mouse.
typedef struct Mouse {
    SDL_Point position;
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

    SDL_Event event;

    _Bool running;

    Keyboard keyboard;
    Mouse mouse;

    Camera camera;

    Dungeon dungeon;
    Entity player;
} Game;

#endif // STRUCTS_H