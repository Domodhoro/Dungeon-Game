#ifndef SETUP_H
#define SETUP_H

// Função responsável por configurar a câmera.
static void setupCamera(Game *game) {
    // Define a posição inicial da câmera.
    game->camera.position = (SDL_Point) {
        11 * 32, 6 * 32
    };
    // Define a velocidade inicial.
    game->camera.speed = 4;
}

// Função responsável por criar e configurar o jogador.
static void setupPlayer(Game *game) {
    // Define o retângulo de origem da textura do jogador.
    game->player.src = (SDL_Rect) {
        0, 0, 32, 32
    };
    // Define a posição e o tamanho do jogador na tela.
    game->player.dst = (SDL_Rect) {
        (SCREEN_WIDTH - PLAYER_WIDTH) / 2, (SCREEN_HEIGHT - PLAYER_HEIGHT) / 2, PLAYER_WIDTH, PLAYER_HEIGHT
    };
    // Configura as coordenadas de textura e posição do coração do jogador.
    game->player.hearts.src = (SDL_Rect) {
        0, 0, 32, 32
    };
    game->player.hearts.dst = (SDL_Rect) {
        SCREEN_WIDTH - HEART_SIZE, 0, HEART_SIZE, HEART_SIZE
    };
    // Ajusta a direção do jogador.
    game->player.direction = NONE;
}

// Função responsável por criar e configurar o inventário do jogador.
static void setupInventory(Game *game) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        game->inventory.src[i] = (SDL_Rect) {
            0, 0, 16, 16
        };
        game->inventory.dst[i] = (SDL_Rect) {
            i * INVENTORY_CELL_SIZE + (SCREEN_WIDTH - 10 * INVENTORY_CELL_SIZE) / 2, SCREEN_HEIGHT - INVENTORY_CELL_SIZE, INVENTORY_CELL_SIZE, INVENTORY_CELL_SIZE
        };
        // Ajustando a posição.
        game->inventory.dst[i].y -= 10;
    }
}

// Função responsável por criar e configurar uma sala da masmorra.
static _Bool addRoomToDungeon(Game *game, const char *roomName, const SDL_Point position) {
    // Cria uma instância da sala.
    Room *newRoom = malloc(sizeof(Room));
    if (!newRoom) {
        fprintf(stderr, "Falha ao alocar mémoria para a sala da masmorra.\n");
        return false;
    }
    // Zera a memória alocada para a estrutura da sala.
    memset(newRoom, 0, sizeof(Room));

    // Acessa a sala específica usando o nome fornecido.
    lua_getfield(game->lua, -1, roomName);
    if (!lua_istable(game->lua, -1)) {
        fprintf(stderr, "Erro ao acessar a sala '%s': %s\n", roomName, lua_tostring(game->lua, -1));
         // Remove a tabela principal e o erro da pilha.
        lua_pop(game->lua, 2);
        return false;
    }

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
                // Em caso de erro na leitura da tabela define o bloco como ar.
                newRoom->block[i][j].type = AIR;
                newRoom->block[i][j].properties.isSolid = false;
            }
            // Remove o valor da pilha.
            lua_pop(game->lua, 1);

            // Configura a posição dos blocos.
            newRoom->block[i][j].dst = (SDL_Rect) { 
                position.x + i * BLOCK_WIDTH, position.y + j * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT
            };
        }
    }

    // Configura a posição da sala.
    newRoom->position = position;

    // Configura o ponteiro para a próxima sala como nulo.
    newRoom->next = NULL;

    // Adiciona a nova sala ao final da lista.
    if (game->dungeon->room == NULL) {
        // Se a dungeon ainda não tem nenhuma sala, define esta como a primeira.
        game->dungeon->room = newRoom;
    } else {
        // Caso contrário, percorre a lista até o final.
        Room *current = game->dungeon->room;
        while (current->next != NULL) {
            current = current->next;
        }
        // Adiciona a nova sala como próxima da última sala.
        current->next = newRoom;
    }
    return true;
}

// Função responsável por criar e configurar uma masmorra.
static _Bool setupDungeon(Game *game) {
    // Cria uma instância da masmorra.
    game->dungeon = malloc(sizeof(Dungeon));
    if (!game->dungeon) {
        fprintf(stderr, "Falha ao alocar mémoria para a masmorra.\n");
        return false;
    }
    // Zera a memória alocada para a estrutura da masmorra.
    memset(game->dungeon, 0, sizeof(Dungeon));

    // Acessa a tabela.
    lua_getglobal(game->lua, "dungeon");
    if (!lua_istable(game->lua, -1)) {
        fprintf(stderr, "Erro ao acessar a tabela 'dungeon': %s\n", lua_tostring(game->lua, -1));
        lua_pop(game->lua, 1);
        return false;
    }

    // Cria uma sala para a masmorra.
    if (!addRoomToDungeon(game, "room_1", (SDL_Point) {0, 64})) {
        return false;
    }
    return true;
}

// Função responsável por criar e configurar o menu principal do jogo.
static _Bool setupMainMenu(Game *game) {
    // Define o espaçamento vertical entre os botões.
    const int buttonIndent_1 = 150;
    const int buttonIndent_2 = 100;

    // Cria o botão 'start'.
    if (!createButton(game, 0, "Start", (SCREEN_HEIGHT - 32) / 2 - buttonIndent_1, BLACK)) {
        return false;
    }

    // Cria o botão 'exit'.
    if (!createButton(game, 1, "Exit", (SCREEN_HEIGHT - 32) / 2 - buttonIndent_2, BLACK)) {
        return false;
    }

    // Cria a versão do jogo.
    game->mainMenu.version = newText(game, "Version 1.0", 0, 0, GREEN);
    if (!game->mainMenu.version.texture) {
        fprintf(stderr, "Falha ao criar texto da versão no menu principal.\n");
        return false;
    }

    // Define o posicionamento do texto da versão no canto inferior
    game->mainMenu.version.dst.x = SCREEN_WIDTH  - game->mainMenu.version.dst.w;
    game->mainMenu.version.dst.y = SCREEN_HEIGHT - game->mainMenu.version.dst.h;

    // Define o posicionamento da textura de fundo na tela.
    game->mainMenu.dst = (SDL_Rect) {
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
    };

    // Texto que mostra a versão do jogo no menu principal.
    game->mainMenu.version = newText(game, "Version 1.0", 0, 0, GREEN);
    if (!game->mainMenu.version.texture) {
        fprintf(stderr, "Falha ao criar texto da versão no menu principal.");
        return false;
    }
    
    // Define o posicionamente da textura no canto inferior.
    game->mainMenu.version.dst.x = SCREEN_WIDTH  - game->mainMenu.version.dst.w;
    game->mainMenu.version.dst.y = SCREEN_HEIGHT - game->mainMenu.version.dst.h;
    return true;
}

// Função responsável por configurar os objetos do jogo.
_Bool setup(Game *game) {    
    // Configura os objetos do jogo.
    setupCamera(game);
    setupPlayer(game);
    setupInventory(game);
    if (!setupDungeon(game) || !setupMainMenu(game)) {
        return false;
    }
    return true;
}

#endif // SETUP_H