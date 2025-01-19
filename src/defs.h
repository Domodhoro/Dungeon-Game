#ifndef DEFS_H
#define DEFS_H

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

#endif // DEFS_H