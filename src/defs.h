#ifndef DEFS_H
#define DEFS_H

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
#define CYAN (SDL_Color) {0, 255, 255}
#define MAGENTA (SDL_Color) {255, 0, 255}
#define ORANGE (SDL_Color) {255, 165, 0}
#define PURPLE (SDL_Color) {128, 0, 128}
#define PINK (SDL_Color) {255, 182, 193}
#define BROWN (SDL_Color) {165, 42, 42}

#endif // DEFS_H