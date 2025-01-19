#ifndef DUNGEON_H
#define DUNGEON_H

// Define o tipo de bloco.
static void setBlockType(Game *game, const int i, const int j) {
	// Lê o tipo de bloco da tabela lua.
	game->dungeon->block[i][j].type = (int)lua_tonumber(game->L, -1);
	// Define as propriedades de cada bloco.
    switch (game->dungeon->block[i][j].type) {
    case AIR:
    	game->dungeon->block[i][j].isSolid = FALSE;
    	break;
    case WOOD:
    	game->dungeon->block[i][j].isSolid = FALSE;
    	break;
    case STONE:
    	game->dungeon->block[i][j].isSolid = TRUE;
    	break;
    }
}

// Função responsável por criar uma masmorra.
BOOL createDungeon(Game *game) {
    // Aloca memória para a estrutura de dados da masmorra.
    game->dungeon = malloc(sizeof(Dungeon));
    if (NOT game->dungeon) {
        fprintf(stderr, "Falha ao alocar memória para a masmorra.\n");
        return FALSE;
    }
    // Zera a memória alocada para a estrutura da masmorra.
    memset(game->dungeon, 0, sizeof(Dungeon));

    // Acessa a tabela principal.
    lua_getglobal(game->L, "dungeon");
    if (NOT lua_istable(game->L, -1)) {
        fprintf(stderr, "Erro ao acessar a tabela 'dungeon': %s\n", lua_tostring(game->L, -1));
        lua_pop(game->L, 1);
        return FALSE;
    }

    // Acessa a sala específica usando o nome fornecido.
    const char *roomName = "room_3";
    lua_getfield(game->L, -1, roomName);
    if (NOT lua_istable(game->L, -1)) {
        fprintf(stderr, "Erro ao acessar a sala '%s': %s\n", roomName, lua_tostring(game->L, -1));
         // Remove a tabela principal e o erro da pilha.
        lua_pop(game->L, 2);
        return FALSE;
    }

    // Preenche a masmorra com blocos a partir dos dados da tabela lua.
    int i;
    int j;
    for (i = 0; i < DUNGEON_WIDTH; i++) {
        for (j = 0; j < DUNGEON_HEIGHT; j++) {
            // Calcula o índice na tabela.
            int index = i * DUNGEON_HEIGHT + j + 1;
            lua_pushnumber(game->L, index);
            lua_gettable(game->L, -2);

            // Verifica se o valor é um número.
            if (lua_isnumber(game->L, -1)) {
                // Define as propriedades de cada bloco.
                setBlockType(game, i, j);
            } else {
            	// Em caso de erro na leitura da tabela define o bloco como ar.
                game->dungeon->block[i][j].type = AIR;
                game->dungeon->block[i][j].isSolid = FALSE;
            }
            // Remove o valor da pilha.
            lua_pop(game->L, 1);

            // Define a origem e o destino para a textura do bloco.
            game->dungeon->block[i][j].src = (SDL_Rect) { 
            	0, 0, 32, 32 
            };
            game->dungeon->block[i][j].dst = (SDL_Rect) { 
            	i * BLOCK_WIDTH, j * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT 
            };
        }
    }

    // Lê o nível da masmorra.
    lua_getfield(game->L, -1, "level");
    if (lua_isnumber(game->L, -1)) {
        game->dungeon->level = (int)lua_tonumber(game->L, -1);
    } else {
        game->dungeon->level = 1;
    }
    // Remove a referência à sala.
    lua_pop(game->L, 1);

    // Carrega a textura da masmorra.
    game->dungeon->texture = IMG_LoadTexture(game->renderer, "./img/dungeon.png");
    if (NOT game->dungeon->texture) {
        fprintf(stderr, "Falha ao carregar texturas da masmorra: %s\n", IMG_GetError());
        return FALSE;
    }
    return TRUE;
}

#endif // DUNGEON_H