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
void setBlockProperties(Game *game, const int i, const int j) {
    // Lê o tipo de bloco da tabela lua.
    int blockType = (int)lua_tonumber(game->L, -1);
    
    // Verifica se o tipo de bloco é válido.
    if (blockType < 0 || blockType >= sizeof(blockProperties) / sizeof(blockProperties[0])) {
        // Em caso de erro caso o tipo de bloco não seja válido.
        return;
    }
    
    // Define as propriedades do bloco de acordo com o tipo.
    game->dungeon->block[i][j].type                    = blockType;
    game->dungeon->block[i][j].properties.src          = blockProperties[blockType].src;
    game->dungeon->block[i][j].properties.isSolid      = blockProperties[blockType].isSolid;
    game->dungeon->block[i][j].properties.isBackground = blockProperties[blockType].isBackground;
}

// Atualiza a textura do jogador com base na direção.
void updatePlayerTexture(Game *game, const DIRECTION direction) {
    switch ((int)direction) {
        case UP:
            game->player.src.x = 0;
            break;
        case DOWN:
            game->player.src.x = 32;
            break;
        case RIGHT:
            game->player.src.x = 32;
            break;
        case LEFT:
            game->player.src.x = 0;
            break;
    }
}

// Criar a superfície com o texto.
Text newText(Game *game, const char *input, int x, int y, SDL_Color color) {
    // Cria uma estrutura de texto.
    Text text;
    SDL_Surface *textSurface = TTF_RenderText_Blended(game->font[0], input, color);
    if (!textSurface) {
        printf("Erro ao criar superfície de texto: %s\n", TTF_GetError());
        return text;
    }

    // Definir o retângulo para renderizar o texto.
    text.dst = (SDL_Rect) {
        x, y, textSurface->w, textSurface->h
    };

    // Criar a textura a partir da superfície.
    text.texture = SDL_CreateTextureFromSurface(game->renderer, textSurface);
    // Liberar a superfície após criar a textura.
    SDL_FreeSurface(textSurface);
    return text;
}

// Função que verifica se o cursor do mouse está dentro da área de um retângulo.
_Bool isCursorInsideRect(const SDL_Point *cursorPosition, const SDL_Rect *dst) {
    if (cursorPosition->x >= dst->x && cursorPosition->x <= dst->x + dst->w && cursorPosition->y >= dst->y && cursorPosition->y <= dst->y + dst->h) {
        // Cursor está dentro do retângulo.
        return true;
    }
    // Cursor está fora do retângulo.
    return false;
}

// Função que verifica a colisão do jogador com os blocos da masmorra.
_Bool checkCollisionWithBlock(Game *game) {
    SDL_Rect blockDst;
    // Laço que percorre todos os blocos da masmorra.
    for (int i = 0; i < DUNGEON_WIDTH; i++) {
        for (int j = 0; j < DUNGEON_HEIGHT; j++) {
            if (game->dungeon->block[i][j].properties.isSolid) {
                blockDst = game->dungeon->block[i][j].dst;

                blockDst.x = i * BLOCK_WIDTH  + game->camera.position.x;
                blockDst.y = j * BLOCK_HEIGHT + game->camera.position.y;

                // Se houver colisão, retorna verdadeiro.
                if (SDL_HasIntersection(&game->player.dst, &blockDst)) {
                    return true;
                }
            }
        }
    }
    // Senão houver colisão, retorna falso.
    return false;
}

// Função responsável por carregar as texturas do jogo.
_Bool loadTexture(Game *game, const char *filePath, const TEXTURE_ID ID) {
    game->textures[ID] = IMG_LoadTexture(game->renderer, filePath);
    if (!game->textures[ID]) {
        fprintf(stderr, "Falha ao carregar textura: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

// Função que carrega a fonte de texto.
_Bool loadFont(Game *game, const char *filePath, const FONT_ID ID, const int fontSize) {
    game->font[ID] = TTF_OpenFont(filePath, fontSize);
    if (!game->font[ID]) {
        fprintf(stderr, "Falha ao carregar a fonte: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

#endif //  UTILS_H