#ifndef INVENTORY_H
#define INVENTORY_H

// Função responsável por criar e configurar o inventário do jogador.
void setupInventory(Game *game) {
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

// Renderiza o inventário do jogador.
void renderInventory(Game *game) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        SDL_RenderCopy(game->renderer, game->textures[INVENTORY_TEXTURE], &game->inventory.src[i], &game->inventory.dst[i]);
    }
}

#endif // INVENTORY_H