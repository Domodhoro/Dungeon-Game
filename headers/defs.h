#ifndef DEFS_H
#define DEFS_H

// Para a fase desenvolvimento do jogo.
#define DEV true

// As vezes é necessário para o SDL.
#define SDL_HANDLED_MAIN

// Define o título da janela do jogo, a largura e a altura da tela (em pixels).
#define SCREEN_TITLE "Dungeon"
#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768

// Define a taxa de quadros por segundo (FPS) desejada.
#define FPS 80
// Esta constante é usado para controlar a taxa de atualização do jogo.
#define FRAME_TIME (1000 / FPS)

// Define o tamanho do jogador.
#define PLAYER_WIDTH  64
#define PLAYER_HEIGHT 64

// Define o tamanho dos corações do jogador.
#define HEART_SIZE 48

// Define o tamanho dos blocos.
#define BLOCK_WIDTH  32
#define BLOCK_HEIGHT 32

// Define o tamanho máximo de uma masmorra.
#define ROOM_WIDTH  32
#define ROOM_HEIGHT 32

// Define o número máximo de botões no menu principal.
#define MAX_MAIN_MENU_BUTTONS 2

// Define o tamanho do inventário.
#define INVENTORY_SIZE 10
// Define o tamanho da celula do inventário.
#define INVENTORY_CELL_SIZE 32

// Define a quantidade de texturas no jogo.
#define MAX_TEXTURES 6
// Define a quantidade de fontes no jogo.
#define MAX_FONTS 1

// Define algumas constantes para cores.
#define WHITE   (SDL_Color) {255, 255, 255}
#define BLACK   (SDL_Color) {  0,   0,   0}
#define RED     (SDL_Color) {255,   0,   0}
#define GREEN   (SDL_Color) {  0, 255,   0}
#define BLUE    (SDL_Color) {  0,   0, 255}
#define YELLOW  (SDL_Color) {255, 255,   0}
#define GRAY    (SDL_Color) { 50,  50,  50}
#define CYAN    (SDL_Color) {  0, 255, 255}
#define MAGENTA (SDL_Color) {255,   0, 255}
#define ORANGE  (SDL_Color) {255, 165,   0}
#define PURPLE  (SDL_Color) {128,   0, 128}
#define PINK    (SDL_Color) {255, 182, 193}
#define BROWN   (SDL_Color) {165,  42,  42}

#endif // DEFS_H