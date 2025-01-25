#ifndef SETUP_H
#define SETUP_H

// Função responsável por criar e configurar uma masmorra.
static _Bool setupDungeon(Game *game, const char *roomName) {
    // Acessa a tabela principal.
    lua_getglobal(game->L, "dungeon");
    if (!lua_istable(game->L, -1)) {
        fprintf(stderr, "Erro ao acessar a tabela 'dungeon': %s\n", lua_tostring(game->L, -1));
        lua_pop(game->L, 1);
        return DEU_MUITO_RUIM;
    }

    // Acessa a sala específica usando o nome fornecido.
    lua_getfield(game->L, -1, roomName);
    if (!lua_istable(game->L, -1)) {
        fprintf(stderr, "Erro ao acessar a sala '%s': %s\n", roomName, lua_tostring(game->L, -1));
         // Remove a tabela principal e o erro da pilha.
        lua_pop(game->L, 2);
        return DEU_MUITO_RUIM;
    }

    // Preenche a masmorra com blocos a partir dos dados da tabela lua.
    for (int i = 0; i < DUNGEON_WIDTH; i++) {
        for (int j = 0; j < DUNGEON_HEIGHT; j++) {
            // Calcula o índice na tabela.
            lua_pushnumber(game->L, j * DUNGEON_HEIGHT + i + 1);
            lua_gettable(game->L, -2);

            // Verifica se o valor é um número.
            if (lua_isnumber(game->L, -1)) {
                // Define as propriedades de cada bloco.
                setBlockProperties(game, i, j);
            } else {
                // Em caso de erro na leitura da tabela define o bloco como ar.
                game->dungeon.block[i][j].type = AIR;
                game->dungeon.block[i][j].properties.isSolid = false;
            }
            // Remove o valor da pilha.
            lua_pop(game->L, 1);

            // COnfigura a posição dos blocos.
            game->dungeon.block[i][j].dst = (SDL_Rect) { 
                i * BLOCK_WIDTH, j * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT 
            };
        }
    }

    // Lê o nível da masmorra.
    lua_getfield(game->L, -1, "level");
    if (lua_isnumber(game->L, -1)) {
        game->dungeon.level = (int)lua_tonumber(game->L, -1);
    } else {
        game->dungeon.level = 1;
    }
    // Remove a referência à sala.
    lua_pop(game->L, 1);
    return true;
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

    // Define a quantidade de pontos de vida do jogador.
    game->player.health = 3;
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

// Função responsável por criar e configurar o menu principal do jogo.
static _Bool setupMainMenu(Game *game) {
    // Define a textura dos botões e o posicionamento na tela.
    const int buttonWidth    = 5 * 32;
    const int buttonHeight   = 32;
    const int buttonIndent_1 = 150;
    const int buttonindent_2 = 100;
    // Botão 'start'.
    game->mainMenu.button[0].src = (SDL_Rect) {
        0, 0, buttonWidth, buttonHeight
    };
    game->mainMenu.button[0].dst = (SDL_Rect) {
        (SCREEN_WIDTH - buttonWidth) / 2, (SCREEN_HEIGHT - buttonHeight) / 2 - buttonIndent_1, buttonWidth, buttonHeight
    };
    // Botão 'exit'.
    game->mainMenu.button[1].src = (SDL_Rect) {
        0, 0, buttonWidth, buttonHeight
    };
    game->mainMenu.button[1].dst = (SDL_Rect) {
        (SCREEN_WIDTH - buttonWidth) / 2, (SCREEN_HEIGHT - buttonHeight) / 2 - buttonindent_2, buttonWidth, buttonHeight
    };

    // Cria os textos para o menu principal.
    game->mainMenu.button[0].text = newText(game, "Start", 0, 0, BLACK);
    game->mainMenu.button[1].text = newText(game, "Exit", 0, 0, BLACK);
    if (!game->mainMenu.button[0].text.texture || !game->mainMenu.button[1].text.texture) {
        fprintf(stderr, "Falha ao criar texto para os botões no menu principal.");
        return DEU_MUITO_RUIM;
    }
    // Posiciona os textos no centro das texturas dos botões.
    game->mainMenu.button[0].text.dst.x = (SCREEN_WIDTH  - game->mainMenu.button[0].text.dst.w) / 2;
    game->mainMenu.button[0].text.dst.y = (SCREEN_HEIGHT - game->mainMenu.button[0].text.dst.h) / 2 - buttonIndent_1;
    game->mainMenu.button[1].text.dst.x = (SCREEN_WIDTH  - game->mainMenu.button[1].text.dst.w) / 2;
    game->mainMenu.button[1].text.dst.y = (SCREEN_HEIGHT - game->mainMenu.button[1].text.dst.h) / 2 - buttonindent_2;

    // Define o posicionamento da textura de fundo na tela.
    game->mainMenu.dst = (SDL_Rect) {
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT
    };

    // Texto que mostra a versão do jogo no menu principal.
    game->mainMenu.version = newText(game, "Version 1.0", 0, 0, GREEN);
    if (!game->mainMenu.version.texture) {
        fprintf(stderr, "Falha ao criar texto da versão no menu principal.");
        return DEU_MUITO_RUIM;
    }
    // Define o posicionamente da textura no canto inferior.
    game->mainMenu.version.dst.x = SCREEN_WIDTH  - game->mainMenu.version.dst.w;
    game->mainMenu.version.dst.y = SCREEN_HEIGHT - game->mainMenu.version.dst.h;
    return true;
}


// Função responsável por configurar os objetos do jogo.
_Bool setup(Game *game) {    
	// Criação dos objetos do jogo.
    setupPlayer(game);
    setupInventory(game);
    if (!setupDungeon(game, "room_1") || !setupMainMenu(game)) {
        finish(game);
        return false;
    }
    return true;
}

#endif // SETUP_H