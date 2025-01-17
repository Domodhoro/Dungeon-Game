#ifndef UPDATE_H
#define UPDATE_H

/*
// https://www.parallelrealities.co.uk/tutorials/adventure/adventure1.php

bool isValidMove(Game* game, int x, int y) {
    // Verifica se as coordenadas estão dentro dos limites da masmorra
    if (x < 0 || x >= DUNGEON_WIDTH || y < 0 || y >= DUNGEON_HEIGHT) {
        return false;  // Fora dos limites
    }

    // Acessa o valor da tabela Lua para a posição (x, y)
    lua_getglobal(game->L, "dungeon");
    int index = y * DUNGEON_WIDTH + x + 1;  // Lua usa index 1, C usa index 0
    lua_pushnumber(game->L, index);
    lua_gettable(game->L, -2);  // Obtém o valor no índice 'index'

    bool valid = false;
    if (lua_isnumber(game->L, -1)) {
        int value = (int)lua_tonumber(game->L, -1);
        if (value != 0) {
            valid = true;  // O bloco é válido (diferente de zero)
        }
    }

    lua_pop(game->L, 1);  // Remove o valor da pilha
    lua_pop(game->L, 1);  // Remove a tabela 'dungeon' da pilha

    return valid;
}

void movePlayer(Game* game, Player* player, int dx, int dy) {
    // Cálculo da nova posição
    int newX = player->x + dx;
    int newY = player->y + dy;

    // Verifica se a nova posição é válida
    if (isValidMove(game, newX, newY)) {
        player->x = newX;
        player->y = newY;
    } else {
        printf("Movimento inválido para a posição (%d, %d)\n", newX, newY);
    }
}

// Supondo que o player tenha uma estrutura 'player' e o jogo tenha um objeto 'game'

if (keyPress == SDLK_UP) {
    movePlayer(game, &player, 0, -1);  // Mover para cima
} else if (keyPress == SDLK_DOWN) {
    movePlayer(game, &player, 0, 1);   // Mover para baixo
} else if (keyPress == SDLK_LEFT) {
    movePlayer(game, &player, -1, 0);  // Mover para a esquerda
} else if (keyPress == SDLK_RIGHT) {
    movePlayer(game, &player, 1, 0);   // Mover para a direita
}
*/

// Escolhe a posição na textura do jogador com base na direção.
static void playerUpdate(Game *game, const DIRECTION direction) {
    // Escolhe a posição na textura do jogador.
    switch (direction) {
        case UP:
            game->player.src.x = 0;
            game->player.src.y = 0;
            break;
        case DOWN:
            game->player.src.x = 32;
            game->player.src.y = 0;
            break;
        case RIGHT:
            game->player.src.x = 32;
            game->player.src.y = 0;
            break;
        case LEFT:
            game->player.src.x = 0;
            game->player.src.y = 0;
            break;
    }
}

// Função que atualiza a lógica do jogo.
void update(Game *game) {
    DIRECTION direction = NONE;

    // Move a câmera para cima.
    if (game->keyboard.states[SDL_SCANCODE_W]) {
        direction = UP;
        // Move a câmera.
        game->camera.position.y += game->player.speed;
    }
    // Move o câmera para a esquerda.
    if (game->keyboard.states[SDL_SCANCODE_A]) {
        direction = LEFT;
        // Move a câmera.
        game->camera.position.x += game->player.speed;
    }
    // Move o câmera para baixo.
    if (game->keyboard.states[SDL_SCANCODE_S]) {
        direction = DOWN;
        // Move a câmera.
        game->camera.position.y -= game->player.speed;
    }
    // Move o câmera para a direita.
    if (game->keyboard.states[SDL_SCANCODE_D]) {
        direction = RIGHT;
        // Move a câmera.
        game->camera.position.x -= game->player.speed;
    }

    playerUpdate(game, direction);
}

#endif // UPDATE_H