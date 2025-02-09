#ifndef MAIN_MENU_H
#define MAIN_MENU_H

// Função para criar e configurar um botão.
static _Bool createButton(Game *game, int buttonIndex, const char *label, int yPosition, SDL_Color textColor) {
    const int buttonWidth  = 5 * 32;
    const int buttonHeight = 32;
    
    // Define a textura do botão.
    game->mainMenu.button[buttonIndex].src = (SDL_Rect) {
        0, 0, buttonWidth, buttonHeight
    };
    
    // Define a posição do botão.
    game->mainMenu.button[buttonIndex].dst = (SDL_Rect) {
        (SCREEN_WIDTH - buttonWidth) / 2, yPosition, buttonWidth, buttonHeight
    };
    
    // Cria o texto para o botão.
    game->mainMenu.button[buttonIndex].text = newText(game, label, 0, 0, textColor);
    if (not game->mainMenu.button[buttonIndex].text.texture) {
        fprintf(stderr, "Falha ao criar texto para o botão %s no menu principal.\n", label);
        return false;
    }
    
    // Posiciona o texto no centro do botão.
    game->mainMenu.button[buttonIndex].text.dst.x = (SCREEN_WIDTH - game->mainMenu.button[buttonIndex].text.dst.w) / 2;
    game->mainMenu.button[buttonIndex].text.dst.y = yPosition + (buttonHeight - game->mainMenu.button[buttonIndex].text.dst.h) / 2;
    return true;
}

// Função responsável por criar e configurar o menu principal do jogo.
_Bool setupMainMenu(Game *game) {
    // Define o espaçamento vertical entre os botões.
    const int buttonIndent_1 = 150;
    const int buttonIndent_2 = 100;

    // Cria o botão 'start'.
    if (not createButton(game, 0, "Start", (SCREEN_HEIGHT - 32) / 2 - buttonIndent_1, BLACK)) {
        return false;
    }

    // Cria o botão 'exit'.
    if (not createButton(game, 1, "Exit", (SCREEN_HEIGHT - 32) / 2 - buttonIndent_2, BLACK)) {
        return false;
    }

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