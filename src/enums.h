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

// Define os tipos de blocos na masmorra.
typedef enum DUNGEON_BLOCKS {
    AIR = 0,
    WOOD,
    STONE
} DUNGEON_BLOCKS;

#endif // ENUMS_H