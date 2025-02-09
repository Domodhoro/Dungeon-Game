#ifndef MAIN_MENU_H
#define MAIN_MENU_H

// Função responsável por criar e configurar o menu principal do jogo.
_Bool setupMainMenu(Game *game) {
    // Define o espaçamento vertical entre os botões.
    const int buttonIndent_1 = 150;
    const int buttonIndent_2 = 100;

    // Cria o botão 'start'.
    game->mainMenu.button[0] = createButton(game, "Start", BLACK, (SCREEN_WIDTH - (5 * 32)) / 2, (SCREEN_HEIGHT - 32) / 2 - buttonIndent_1, 5 * 32, 32);
    // Cria o botão 'exit'.
    game->mainMenu.button[1] = createButton(game, "Exit", BLACK, (SCREEN_WIDTH - (5 * 32)) / 2, (SCREEN_HEIGHT - 32) / 2 - buttonIndent_2, 5 * 32, 32);

    // Cria a versão do jogo.
    game->mainMenu.version = newText(game, "Version 1.0", 0, 0, GREEN);
    if (not game->mainMenu.version.texture) {
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
    if (not game->mainMenu.version.texture) {
        fprintf(stderr, "Falha ao criar texto da versão no menu principal.");
        return false;
    }
    
    // Define o posicionamente da textura no canto inferior.
    game->mainMenu.version.dst.x = SCREEN_WIDTH  - game->mainMenu.version.dst.w;
    game->mainMenu.version.dst.y = SCREEN_HEIGHT - game->mainMenu.version.dst.h;
    return true;
}

// Renderiza o menu principal.
void renderMainMenu(Game *game) {
    // Renderiza o fundo do menu principal.
    SDL_RenderCopy(game->renderer, game->textures[MAIN_MENU_BACKGROUND_TEXTURE], NULL, &game->mainMenu.dst);
    // Renderiza os botões do menu principal.
    for (int i = 0; i < MAX_MAIN_MENU_BUTTONS; i++) {
        SDL_RenderCopy(game->renderer, game->textures[MAIN_MENU_TEXTURE], &game->mainMenu.button[i].src, &game->mainMenu.button[i].dst);
        SDL_RenderCopy(game->renderer, game->mainMenu.button[i].text.texture, NULL, &game->mainMenu.button[i].text.dst);
    }
    // Renderiza o texto com a versão do jogo.
    SDL_RenderCopy(game->renderer, game->mainMenu.version.texture, NULL, &game->mainMenu.version.dst);
}

#endif // MAIN_MENU_H