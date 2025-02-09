#ifndef UPDATE_H
#define UPDATE_H

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

            // Função responsável por atualizar a posição dos blocos da masmorra.
            updateDungeon(game);
            break;
        case PAUSE:

            break;
    }
}

#endif // UPDATE_H