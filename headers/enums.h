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
	AIR     = 0,
    FLOOR_1 = 1,
    FLOOR_2 = 2,
    WALL_1  = 3,
    WALL_2  = 4,
    WALL_3  = 5,
    WALL_4  = 6,
    WALL_5  = 7,
    WALL_6  = 8,
    WALL_7  = 9,
    WALL_8  = 10,
    WALL_9  = 11,
    WALL_10 = 12,
    WALL_11 = 13,
    WALL_12 = 14,
    WALL_13 = 15
} BLOCK_TYPE;

#endif // ENUMS_H