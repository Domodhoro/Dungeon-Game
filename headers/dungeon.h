#ifndef DUNGEON_H
#define DUNGEON_H

// Função responsável por criar e inicializar uma nova sala.
static Room *createAndInitializeRoom(Game *game, const char *roomName, const SDL_Point position) {
    // Cria uma instância da sala.
    Room *newRoom = malloc(sizeof(Room));
    if (not newRoom) {
        fprintf(stderr, "Falha ao alocar memória para a sala da masmorra.\n");
        return NULL;
    }
    // Zera a memória alocada para a estrutura da sala.
    memset(newRoom, 0, sizeof(Room));

    // Acessa a sala específica usando o nome fornecido.
    lua_getfield(game->lua, -1, roomName);
    if (not lua_istable(game->lua, -1)) {
        fprintf(stderr, "Erro ao acessar a tabela da sala '%s': %s\n", roomName, lua_tostring(game->lua, -1));
        lua_pop(game->lua, 2);
        free(newRoom);
        return NULL;
    }

    // Configura a posição da sala.
    newRoom->position = position;
    // Configura o ponteiro para a próxima sala como nulo.
    newRoom->next = NULL;
    return newRoom;
}

// Função responsável por configurar os blocos de uma sala.
static void configureRoomBlocks(Game *game, Room *newRoom) {
    // Preenche a masmorra com blocos a partir dos dados da tabela lua.
    for (int i = 0; i < ROOM_WIDTH; i++) {
        for (int j = 0; j < ROOM_HEIGHT; j++) {
            // Calcula o índice na tabela.
            lua_pushnumber(game->lua, j * ROOM_HEIGHT + i + 1);
            lua_gettable(game->lua, -2);

            // Verifica se o valor é um número.
            if (lua_isnumber(game->lua, -1)) {
                // Define as propriedades de cada bloco.
                setBlockProperties(newRoom, game->lua, i, j);
            } else {
                // Em caso de erro na leitura da tabela, define o bloco como ar.
                newRoom->block[i][j].type = AIR;
                newRoom->block[i][j].properties.isSolid = false;
            }

            // Remove o valor da pilha.
            lua_pop(game->lua, 1);

            // Configura a posição dos blocos.
            newRoom->block[i][j].dst = (SDL_Rect) { 
                newRoom->position.x + i * BLOCK_WIDTH, newRoom->position.y + j * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT
            };
        }
    }
}

// Função responsável por adicionar uma sala à masmorra.
static _Bool addRoomToDungeon(Game *game, const char *roomName, const SDL_Point position) {
    // Cria e inicializa a sala.
    Room *newRoom = createAndInitializeRoom(game, roomName, position);
    if (newRoom == NULL) {
        return false;
    }

    // Configura os blocos da sala.
    configureRoomBlocks(game, newRoom);

    // Adiciona a nova sala ao final da lista.
    if (game->dungeon->room == NULL) {
        // Se a dungeon ainda não tem nenhuma sala, define esta como a primeira.
        game->dungeon->room = newRoom;
    } else {
        // Caso contrário, percorre a lista até o final.
        Room *current = game->dungeon->room;
        while (current->next not_eq NULL) {
            current = current->next;
        }
        // Adiciona a nova sala como próxima da última sala.
        current->next = newRoom;
    }
    return true;
}

// Função responsável por criar e configurar uma masmorra.
_Bool setupDungeon(Game *game) {
    // Cria uma instância da masmorra.
    game->dungeon = malloc(sizeof(Dungeon));
    if (not game->dungeon) {
        fprintf(stderr, "Falha ao alocar mémoria para a masmorra.\n");
        return false;
    }
    // Zera a memória alocada para a estrutura da masmorra.
    memset(game->dungeon, 0, sizeof(Dungeon));

    // Acessa a tabela.
    lua_getglobal(game->lua, "dungeon");
    if (not lua_istable(game->lua, -1)) {
        fprintf(stderr, "Erro ao acessar a tabela 'dungeon': %s\n", lua_tostring(game->lua, -1));
        lua_pop(game->lua, 1);
        return false;
    }

    // Cria uma sala para a masmorra.
    if (not addRoomToDungeon(game, "room_1", (SDL_Point) {0, 64})) {
        return false;
    }
    return true;
}

// Função responsável por atualizar a posição dos blocos da masmorra.
void updateDungeon(Game *game) {
    // Começa pela primeira sala da masmorra.
    Room *currentRoom = game->dungeon->room;
    // Enquanto houver uma sala na lista.
    while (currentRoom not_eq NULL) {
        // Atualiza a posição dos blocos somente se não houver colisão.
        for (int i = 0; i < ROOM_WIDTH; i++) {
            for (int j = 0; j < ROOM_HEIGHT; j++) {
                SDL_Point newPosition = currentRoom->position;

                newPosition.x += i * BLOCK_WIDTH  + game->camera.position.x;
                newPosition.y += j * BLOCK_HEIGHT + game->camera.position.y;

                currentRoom->block[i][j].dst.x = newPosition.x;
                currentRoom->block[i][j].dst.y = newPosition.y;
            }
        }   

        // Vai para a próxima sala na lista.
        currentRoom = currentRoom->next;
    }
}

// Função para renderizar a masmorra.
void renderDungeonLayer(Game *game, const _Bool isBackground) {
    // Começa pela primeira sala da masmorra.
    Room *currentRoom = game->dungeon->room;
    // Enquanto houver uma sala na lista.
    while (currentRoom not_eq NULL) {
        for (int i = 0; i < ROOM_WIDTH; i++) {
            for (int j = 0; j < ROOM_HEIGHT; j++) {
                // Verifica se o bloco não é de ar e se o bloco deverá estar atrás ou na frente do jogador.
                if (game->dungeon->room->block[i][j].type != AIR && game->dungeon->room->block[i][j].properties.isBackground == isBackground) {
                    // Renderiza o bloco na tela.
                    SDL_RenderCopy(game->renderer, game->textures[DUNGEON_TEXTURE], &game->dungeon->room->block[i][j].properties.src, &game->dungeon->room->block[i][j].dst);   
                }
            }
        }

        // Vai para a próxima sala na lista.
        currentRoom = currentRoom->next;
    }
}

// Função que destrói as salas e a masmorra.
void destroyDungeon(Game *game) {
    if (game->dungeon) {
        // Começa pela primeira sala da masmorra.
        Room *currentRoom = game->dungeon->room;
        // Itera sobre todas as salas.
        while (currentRoom not_eq NULL) {
            // Guarda o ponteiro para a próxima sala.
            Room *nextRoom = currentRoom->next;
            // Libera a memória da sala atual.
            free(currentRoom);
            // Avança para a próxima sala.
            currentRoom = nextRoom;
        }

        // Após liberar todas as salas, zera o ponteiro para a lista de salas e libera a memória da masmorra.
        game->dungeon->room = NULL;
        free(game->dungeon);
        game->dungeon = NULL;
    }
}

#endif //  DUNGEON_H