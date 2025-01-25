#ifndef ENUMS_H
#define ENUMS_H

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
    WALL_1 = 2,
    WALL_2,
    DOOR_1 = 4,
    DOOR_2,
    DOOR_3,
    DOOR_4
} BLOCK_TYPE;

#endif // ENUMS_H