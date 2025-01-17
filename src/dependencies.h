#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H

// Bibliotecas padrão do C.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Bibliotecas SDL2.
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

// Bibliotecas Lua
#ifdef __linux__
    #include "../lua54/lua.h"
    #include "../lua54/lualib.h"
    #include "../lua54/lauxlib.h"
#elif _WIN32
	#include <lua54/lua.hpp>
#endif

#endif // DEPENDENCIES_H