#ifndef ENUMS_H
#define ENUMS_H

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