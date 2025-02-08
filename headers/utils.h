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
void setBlockProperties(Room *room, lua_State *L, const int i, const int j) {
    // Lê o tipo de bloco da tabela lua.
    int blockType = (int)lua_tonumber(L, -1);
    
    // Verifica se o tipo de bloco é válido.
    if (blockType < 0 || blockType >= sizeof(blockProperties) / sizeof(blockProperties[0])) {
        // Em caso de erro caso o tipo de bloco não seja válido.
        return;
    }
    
    // Define as propriedades do bloco de acordo com o tipo.
    room->block[i][j].type                    = blockType;
    room->block[i][j].properties.src          = blockProperties[blockType].src;
    room->block[i][j].properties.isSolid      = blockProperties[blockType].isSolid;
    room->block[i][j].properties.isBackground = blockProperties[blockType].isBackground;
}

// Atualiza a textura do jogador com base na direção.
void updatePlayerAnimation(Game *game) {
    switch ((int)game->player.direction) {
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

    // Começa pela primeira sala da masmorra.
    Room *currentRoom = game->dungeon->room;

     // Enquanto houver uma sala na lista.
    while (currentRoom != NULL) {
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

// Função responsável por configurar o ícone da janela de visualização do jogo.
_Bool loadWindowIcon(Game *game, const char *filePath) {
    // Carrega a imagem do ícone da janela e caso ocorra erro ao carregar o ícone, exibe uma mensagem de erro.
    SDL_Surface *iconSurface = IMG_Load(filePath);
    if (!iconSurface) {
        fprintf(stderr, "Falha ao carregar o ícone da janela de visualização: %s\n", IMG_GetError());
        return false;
    }
    SDL_SetWindowIcon(game->window, iconSurface);
    // Libera a superfície que foi usada para o ícone.
    SDL_FreeSurface(iconSurface);
    iconSurface = NULL;
    return true;
}

// Função responsável por carregar a textura do cursor do mouse.
_Bool loadCursorTexture(Game *game, const char *filePath) {
    
    return true;
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

// Função para criar e configurar um botão.
_Bool createButton(Game *game, int buttonIndex, const char *label, int yPosition, SDL_Color textColor) {
    const int buttonWidth  = 5 * 32;
    const int buttonHeight = 32;
    
    // Define a textura do botão.
    game->mainMenu.button[buttonIndex].src = (SDL_Rect) {
        0, 0, buttonWidth, buttonHeight
    };
    
    // Define a posição do botão.
    game->mainMenu.button[buttonIndex].dst = (SDL_Rect) {
        (SCREEN_WIDTH - buttonWidth) / 2, yPosition, buttonWidth, buttonHeight
    };
    
    // Cria o texto para o botão.
    game->mainMenu.button[buttonIndex].text = newText(game, label, 0, 0, textColor);
    if (!game->mainMenu.button[buttonIndex].text.texture) {
        fprintf(stderr, "Falha ao criar texto para o botão %s no menu principal.\n", label);
        return false;
    }
    
    // Posiciona o texto no centro do botão.
    game->mainMenu.button[buttonIndex].text.dst.x = (SCREEN_WIDTH - game->mainMenu.button[buttonIndex].text.dst.w) / 2;
    game->mainMenu.button[buttonIndex].text.dst.y = yPosition + (buttonHeight - game->mainMenu.button[buttonIndex].text.dst.h) / 2;
    return true;
}


#endif // UTILS_H