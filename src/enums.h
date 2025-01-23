#ifndef ENUMS_H
#define ENUMS_H

// Estados do programa.
typedef enum GAME_STATE {
	MAIN_MENU = 0,
	ACTIVE
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

#endif // ENUMS_H