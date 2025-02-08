#ifndef STRUCTS_H
#define STRUCTS_H

// Define a câmera do jogo.
typedef struct Camera {
    SDL_Point position;
    int speed;
} Camera;

// Estrutura para armazenar as propriedades de cada tipo de bloco.
typedef struct BlockProperties {
    SDL_Rect src;
    _Bool isSolid, isBackground;
} BlockProperties;

// Estrututa que representa um bloco.
typedef struct Block {
    BlockProperties properties;
    SDL_Rect dst;
    BLOCK_TYPE type;
} Block;

// Estrututa que representa uma sala da masmorra.
typedef struct Room {
    Block block[ROOM_WIDTH][ROOM_HEIGHT];
    SDL_Point position;
    struct Room* next;
} Room;

// Estrutura que representa uma masmorra.
typedef struct Dungeon {
    Room *room;
} Dungeon;

// Estrutura que representa os pontos de vida do jogador.
typedef struct Heart {
    SDL_Rect src, dst;
} Heart;

// Estrutura que representa o jogador.
typedef struct Player {
    SDL_Rect src, dst;
    Heart hearts;
    DIRECTION direction;
} Player;

// Estrututa que representa o inventário do jogador.
typedef struct Inventory {
    SDL_Rect src[INVENTORY_SIZE], dst[INVENTORY_SIZE];
} Inventory;

// Estrutura para texto.
typedef struct Text {
    SDL_Texture *texture;
    SDL_Rect dst;
} Text;

// Estrutura que representa um botão.
typedef struct Button {
    SDL_Rect src, dst;
    Text text;
} Button;

// Estrutura para o menu principal do jogo.
typedef struct MainMenu {
    Button button[MAX_MAIN_MENU_BUTTONS];
    SDL_Rect dst;
    Text version;
} MainMenu;

// Estrutura que armazena os estados do mouse.
typedef struct Mouse {
    SDL_Point position;
    _Bool leftButton, rightButton;
} Mouse;

// Estrutura que armazena os estados das teclas do teclado.
typedef struct Keyboard {
    const Uint8 *states;
} Keyboard;

// Estrutura que armazena todos os estados das entradas do usuário.
typedef struct UserInputs {
    Mouse mouse;
    Keyboard keyboard;
} UserInputs;

// Estrutura principal que representa o jogo.
typedef struct Game {
    GAME_STATE state;

    lua_State *lua;

    SDL_Window *window;

    SDL_Renderer *renderer;
    SDL_Color backgroundColor;
    SDL_Rect viewport;

    SDL_Event event;

    _Bool isRunning;
    Uint32 frameStart, frameTime;

    SDL_Texture *textures[MAX_TEXTURES];
    TTF_Font *font[MAX_FONTS];

    UserInputs userInputs;
    Camera camera;
    MainMenu mainMenu;
    Player player;
    Inventory inventory;

    Dungeon *dungeon;
} Game;

#endif // STRUCTS_H