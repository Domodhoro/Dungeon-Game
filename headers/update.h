#ifndef UPDATE_H
#define UPDATE_H

// Função responsável por mover a câmera e atualizar a direção do jogador.
static void updateCamera(Game *game) {
    // Move a câmera para cima.
    if (game->userInputs.keyboard.states[SDL_SCANCODE_W]) {
        game->player.direction = UP;
        game->camera.position.y += game->camera.speed;
    }
    // Move a câmera para a esquerda.
    if (game->userInputs.keyboard.states[SDL_SCANCODE_A]) {
        game->player.direction = LEFT;
        game->camera.position.x += game->camera.speed;
    }
    // Move a câmera para baixo.
    if (game->userInputs.keyboard.states[SDL_SCANCODE_S]) {
        game->player.direction = DOWN;
        game->camera.position.y -= game->camera.speed;
    }
    // Move a câmera para a direita.
    if (game->userInputs.keyboard.states[SDL_SCANCODE_D]) {
        game->player.direction = RIGHT;
        game->camera.position.x -= game->camera.speed;
    }
}

// Função responsável por atualizar o estado do jogo.
void update(Game *game) {
    // Armazena a posição da câmera antes do movimento.
    SDL_Point cameraPrev = game->camera.position;

    switch (game->state) {
        case MAIN_MENU:
            // Verifica qual dos botões do menu principal foi clicado.
            for (int i = 0; i < MAX_MAIN_MENU_BUTTONS; i++) {
                if (isCursorInsideRect(&game->userInputs.mouse.position, &game->mainMenu.button[i].dst)) {
                    game->mainMenu.button[i].src.y = 32;
                    if (game->userInputs.mouse.leftButton) {
                        if (i == 0) {
                            game->state = ACTIVE;
                        } else if (i == 1) {
                            game->isRunning = false;
                        }
                    }
                } else {
                    game->mainMenu.button[i].src.y = 0;
                }
            }
            break;
        case ACTIVE:
            // Move a câmera e atualizar a direção do jogador.
            updateCamera(game);
            // Atualiza a textura do jogador.
            updatePlayerAnimation(game);

            // Verifica colisão do jogador com os blocos da masmorra e se houver, retorna para a posição anterior.
            if (checkCollisionWithBlock(game)) {
                game->camera.position = cameraPrev;
            }

            // Começa pela primeira sala da masmorra.
            Room *currentRoom = game->dungeon->room;

            // Enquanto houver uma sala na lista.
            while (currentRoom != NULL) {
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
            break;
        case PAUSE:

            break;
    }
}

#endif // UPDATE_H