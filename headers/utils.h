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
    game->dungeon.block[i][j].type                    = blockType;
    game->dungeon.block[i][j].properties.src          = blockProperties[blockType].src;
    game->dungeon.block[i][j].properties.isSolid      = blockProperties[blockType].isSolid;
    game->dungeon.block[i][j].properties.isBackground = blockProperties[blockType].isBackground;
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
    SDL_Surface *textSurface = TTF_RenderText_Blended(game->font, input, color);
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

// Função que carrega as texturas do jogo e as adiciona a uma lista encadeada.
void loadTexture(Game *game, const char *filePath, const char *name) {
    // Cria um novo nó para a textura.
    Texture *texture = malloc(sizeof(Texture));
    if (!texture) {
        // Verifica se a alocação de memória falhou.
        fprintf(stderr, "Falha na alocação de memória para textura.\n");
        return;
    }
    // Zera a memória alocada para garantir que todos os campos estejam limpos.
    memset(texture, 0, sizeof(Texture));

    // Carrega a textura com o caminho fornecido.
    texture->texture = IMG_LoadTexture(game->renderer, filePath);
    if (!texture->texture) {
        // Caso a textura não tenha sido carregada com sucesso, libera a memória e retorna.
        fprintf(stderr, "Falha ao carregar a textura: %s\n", IMG_GetError());
        free(texture);
        texture = NULL;
        return;
    }

    // Define o nome da textura.
    texture->name = name;
    // Aponta para o primeiro nó da lista existente.
    texture->next = game->texture;
    // Atualiza a cabeça da lista para o novo nó.
    game->texture = texture;
}

// Função que retorna a textura pelo nome fornecido.
SDL_Texture *getTexture(Game *game, const char *name) {
    // Percorre a lista encadeada de texturas.
    Texture *current = game->texture;
    while (current != NULL) {
        // Compara o nome da textura com o nome buscado.
        if (strcmp(current->name, name) == 0) {
            // Retorna a textura se o nome coincidir.
            return current->texture;
        }
        current = current->next;
    }

    // Se não encontrar a textura, retorna NULL.
    fprintf(stderr, "Textura com o nome '%s' não encontrada.\n", name);
    return NULL;
}

#endif //  UTILS_H