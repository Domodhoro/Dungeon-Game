#ifndef CAMERA_H
#define CAMERA_H

// Função responsável por configurar a câmera.
void setupCamera(Game *game) {
    // Define a posição inicial da câmera.
    game->camera.position = (SDL_Point) {
        11 * 32, 6 * 32
    };
    // Define a velocidade inicial.
    game->camera.speed = 4;
}

// Função responsável por mover a câmera e atualizar a direção do jogador.
void updateCamera(Game *game) {
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

#endif // CAMERA_H