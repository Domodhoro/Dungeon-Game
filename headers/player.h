#ifndef PLAYER_H
#define PLAYER_H

// Função responsável por criar e configurar o jogador.
void setupPlayer(Game *game) {
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

// Renderiza o jogador.
void renderPlayer(Game *game) {
    // Renderiza o jogador.
    SDL_RenderCopy(game->renderer, game->textures[PLAYER_TEXTURE], &game->player.src, &game->player.dst);
    // Renderiza os corações do jogador.
    SDL_RenderCopy(game->renderer, game->textures[HEART_TEXTURE], &game->player.hearts.src, &game->player.hearts.dst);
}

#endif // PLAYER_H