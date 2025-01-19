#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H

// Bibliotecas padrão do C.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

// Bibliotecas SDL2.
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

// Bibliotecas lua (Linux e Windows).
#ifdef __linux__
    #include "../lua54/lua.h"
    #include "../lua54/lualib.h"
    #include "../lua54/lauxlib.h"
#elif _WIN32
	#include <lua/lua.h>
	#include <lua/lualib.h>
	#include <lua/lauxlib.h>
#endif

// Incluindo os cabeçalhos necessários para o jogo.
#include "../src/defs.h"
#include "../src/enums.h"
#include "../src/structs.h"
#include "../src/render.h"
#include "../src/update.h"
#include "../src/handleEvents.h"
#include "../src/dungeon.h"
#include "../src/finish.h"
#include "../src/init.h"

#endif // DEPENDENCIES_H