#ifndef UTILS_H
#define UTILS_H

// Inicializar um array com as propriedades de cada tipo de bloco.
static const BlockProperties blockProperties[] = {
    [AIR]    = {{ 0,  0,  0,  0}, false},
    [WOOD]   = {{ 0,  0, 16, 16}, false},
    [WALL_1] = {{ 0, 16, 16, 16}, true },
    [WALL_2] = {{ 0, 32, 16, 16}, true },
    [DOOR_1] = {{ 0, 48, 16, 16}, true },
    [DOOR_2] = {{16, 48, 16, 16}, true },
    [DOOR_3] = {{ 0, 64, 16, 16}, true },
    [DOOR_4] = {{16, 64, 16, 16}, true }
};

// Define as propriedades de cada bloco.
void setBlockProperties(Game *game, const int i, const int j) {
    // Lê o tipo de bloco da tabela lua.
    int blockType = (int)lua_tonumber(game->L, -1);
    
    // Verifica se o tipo de bloco é válido.
    if (blockType < 0 || blockType >= sizeof(blockProperties) / sizeof(blockProperties[0])) {
        // Em caso de erro caso o tipo de bloco não seja válido.
        return;
    }
    
    // Define as propriedades do bloco de acordo com o tipo.
    game->dungeon.block[i][j].type               = blockType;
    game->dungeon.block[i][j].properties.src     = blockProperties[blockType].src;
    game->dungeon.block[i][j].properties.isSolid = blockProperties[blockType].isSolid;
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
    if (cursorPosition->x >= dst->x && cursorPosition->x <= dst->x + dst->w) {
        if (cursorPosition->y >= dst->y && cursorPosition->y <= dst->y + dst->h) {
            // Cursor está dentro do retângulo.
            return true;
        }
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
            if (game->dungeon.block[i][j].properties.isSolid) {
                blockDst = game->dungeon.block[i][j].dst;

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

#endif //  UTILS_H