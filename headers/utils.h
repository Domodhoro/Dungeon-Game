#ifndef UTILS_H
#define UTILS_H

// Matriz que armazena as coordenadas de textura dos blocos da masmorra.
static const BlockProperties blockProperties[] = {
    // A tabela descreve os valores de src, isSolid e isBackground.
    [AIR    ] = {{  0,   0,   0,   0}, false,  true},
    [FLOOR_1] = {{  0,   0,  16,  16}, false,  true},
    [WALL_1 ] = {{  0,  16,  16,  16}, false,  true},
    [WALL_2 ] = {{ 16,  16,  16,  16}, false,  true},
    [WALL_3 ] = {{ 32,  16,  16,  16},  true,  true},
    [WALL_4 ] = {{ 48,  16,  16,  16},  true,  true},
    [WALL_5 ] = {{ 64,  16,  16,  16},  true,  true},
    [WALL_6 ] = {{ 80,  16,  16,  16},  true,  true},
    [WALL_7 ] = {{ 96,  16,  16,  16},  true,  true},
    [WALL_8 ] = {{112,  16,  16,  16}, false, false},
    [WALL_9 ] = {{128,  16,  16,  16},  true,  true},
    [WALL_10] = {{144,  16,  16,  16},  true,  true},
    [WALL_11] = {{160,  16,  16,  16},  true,  true},
    [WALL_12] = {{176,  16,  16,  16},  true,  true},
    [WALL_13] = {{192,  16,  16,  16},  true,  true}
};

// Configura as propriedades de cada bloco.
void setBlockProperties(Room *room, lua_State *L, int i, int j) {
    // Lê o tipo de bloco da tabela lua.
    int blockType = (int)lua_tonumber(L, -1);
    
    // Verifica se o tipo de bloco é válido.
    if (blockType < 0 or blockType >= sizeof(blockProperties) / sizeof(blockProperties[0])) {
        // Em caso de erro caso o tipo de bloco não seja válido.
        return;
    }
    
    // Define as propriedades do bloco de acordo com o tipo.
    room->block[i][j].type                    = blockType;
    room->block[i][j].properties.src          = blockProperties[blockType].src;
    room->block[i][j].properties.isSolid      = blockProperties[blockType].isSolid;
    room->block[i][j].properties.isBackground = blockProperties[blockType].isBackground;
}

// Função que verifica se o cursor do mouse está dentro da área de um retângulo.
_Bool isCursorInsideRect(const SDL_Point *cursorPosition, const SDL_Rect *dst) {
    if (cursorPosition->x >= dst->x and cursorPosition->x <= dst->x + dst->w and cursorPosition->y >= dst->y and cursorPosition->y <= dst->y + dst->h) {
        // Cursor está dentro do retângulo.
        return true;
    }
    // Cursor está fora do retângulo.
    return false;
}

// Função que verifica a colisão do jogador com os blocos da masmorra.
_Bool checkCollisionWithBlock(Game *game) {
    SDL_Rect blockDst;

    // Começa pela primeira sala da masmorra.
    Room *currentRoom = game->dungeon->room;
     // Enquanto houver uma sala na lista.
    while (currentRoom not_eq NULL) {
        // Laço que percorre todos os blocos da masmorra.
        for (int i = 0; i < ROOM_WIDTH; i++) {
            for (int j = 0; j < ROOM_HEIGHT; j++) {
                if (currentRoom->block[i][j].properties.isSolid) {
                    blockDst = currentRoom->block[i][j].dst;

                    blockDst.x = i * BLOCK_WIDTH  + game->camera.position.x;
                    blockDst.y = j * BLOCK_HEIGHT + game->camera.position.y;

                    blockDst.x += currentRoom->position.x;
                    blockDst.y += currentRoom->position.y;

                    // Se houver colisão, retorna verdadeiro.
                    if (SDL_HasIntersection(&game->player.dst, &blockDst)) {
                        return true;
                    }
                }
            }
        }
        // Vai para a próxima sala na lista.
        currentRoom = currentRoom->next;
    }
    // Senão houver colisão, retorna falso.
    return false;
}

#endif // UTILS_H