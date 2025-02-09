#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H

// Biblioteca padrão do C.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// Para alguns teclados.
#include <iso646.h>

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

// Incluindo outros cabeçalhos necessários para o jogo.
#include "../headers/defs.h"
#include "../headers/enums.h"
#include "../headers/structs.h"
#include "../headers/texture.h"
#include "../headers/text.h"
#include "../headers/utils.h"
#include "../headers/mainMenu.h"
#include "../headers/camera.h"
#include "../headers/inventory.h"
#include "../headers/player.h"
#include "../headers/dungeon.h"
#include "../headers/handleEvents.h"
#include "../headers/update.h"
#include "../headers/render.h"
#include "../headers/finish.h"
#include "../headers/setup.h"
#include "../headers/window.h"
#include "../headers/init.h"

#endif // DEPENDENCIES_H