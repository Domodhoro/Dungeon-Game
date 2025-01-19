#ifndef UPDATE_H
#define UPDATE_H

// Atualiza a textura do jogador com base na direção.
static void updatePlayerTexture(Game *game, const DIRECTION direction) {
    switch ((int)direction) {
        case UP:
            game->player->src.x = 0;
            game->player->src.y = 0;
            break;
        case DOWN:
            game->player->src.x = 32;
            game->player->src.y = 0;
            break;
        case RIGHT:
            game->player->src.x = 32;
            game->player->src.y = 0;
            break;
        case LEFT:
            game->player->src.x = 0;
            game->player->src.y = 0;
            break;
    }
}

// Verifica colisões.
static void checkCollision(Game *game, const SDL_Point cameraPrev) {
    for (int i = 0; i < DUNGEON_WIDTH; i++) {
        for (int j = 0; j < DUNGEON_HEIGHT; j++) {
            if (game->dungeon->block[i][j].type != AIR) {
                if (game->dungeon->block[i][j].isSolid) {
                    SDL_Rect blockDst = game->dungeon->block[i][j].dst;

                    blockDst.x = i * BLOCK_WIDTH + game->camera.position.x;
                    blockDst.y = j * BLOCK_HEIGHT + game->camera.position.y;

                    if (SDL_HasIntersection(&game->player->dst, &blockDst)) {
                        // Se houver colisão, desfaz o movimento antes de atualizar os blocos.
                        game->camera.position = cameraPrev;
                        // Sai do loop assim que detectar a colisão.
                        break;
                    }
                }
            }
        }
    }
}

// Função responsável por atualizar o estado do jogo.
void update(Game *game) {
    DIRECTION direction = NONE;

    // Armazena a posição da câmera antes do movimento.
    SDL_Point cameraPrev = game->camera.position;

    // Move a câmera para cima.
    if (game->keyboard.states[SDL_SCANCODE_W]) {
        direction = UP;
        game->camera.position.y += game->camera.speed;
    }
    // Move a câmera para a esquerda.
    if (game->keyboard.states[SDL_SCANCODE_A]) {
        direction = LEFT;
        game->camera.position.x += game->camera.speed;
    }
    // Move a câmera para baixo.
    if (game->keyboard.states[SDL_SCANCODE_S]) {
        direction = DOWN;
        game->camera.position.y -= game->camera.speed;
    }
    // Move a câmera para a direita.
    if (game->keyboard.states[SDL_SCANCODE_D]) {
        direction = RIGHT;
        game->camera.position.x -= game->camera.speed;
    }

    // Verifica se há colisão antes de atualizar os blocos.
    checkCollision(game, cameraPrev);

    // Atualiza a posição dos blocos somente se não houver colisão.
    for (int i = 0; i < DUNGEON_WIDTH; i++) {
        for (int j = 0; j < DUNGEON_HEIGHT; j++) {
            game->dungeon->block[i][j].dst.x = i * BLOCK_WIDTH + game->camera.position.x;
            game->dungeon->block[i][j].dst.y = j * BLOCK_HEIGHT + game->camera.position.y;
        }
    }

    // Atualiza a textura do jogador.
    updatePlayerTexture(game, direction);
}

#endif // UPDATE_H