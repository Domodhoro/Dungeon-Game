#ifndef HANDLE_EVENTS_H
#define HANDLE_EVENTS_H

// Função que trata dos eventos de tecla pressionada do teclado.
static void handleKeyDown(Game *game) {
#if DEV
    // Se a tecla pressionada for ESC, encerra o jogo.
    if (game->userInputs.keyboard.states[SDL_SCANCODE_ESCAPE]) {
        game->isRunning = false;
    }
#endif // DEV
}

// Função que trata dos eventos de tecla solta do teclado.
static void handleKeyUp(Game *game) {

}

// Função que trata dos eventos de botão pressionado do mouse.
static void handleMouseButtonsDown(Game *game) {
    if (game->event.button.button == SDL_BUTTON_LEFT) {
        // Botão esquerdo pressionado.
        game->userInputs.mouse.leftButton = true;
    } else if (game->event.button.button == SDL_BUTTON_RIGHT) {
        // Botão direito pressionado.
        game->userInputs.mouse.rightButton = true;
    }
}

// Função que trata dos eventos de botão solto do mouse.
static void handleMouseButtonsUp(Game *game) {
    if (game->event.button.button == SDL_BUTTON_LEFT) {
        // Botão esquerdo solto.
        game->userInputs.mouse.leftButton = false;
    } else if (game->event.button.button == SDL_BUTTON_RIGHT) {
        // Botão direito solto.
        game->userInputs.mouse.rightButton = false;
    }
}

// Função que trata os eventos.
void handleEvents(Game *game) {
    // Atualiza o estado atual do teclado.
    game->userInputs.keyboard.states = SDL_GetKeyboardState(NULL);
    
    // Retorna as coordenadas atuais do cursor do mouse.
    SDL_GetMouseState(&game->userInputs.mouse.position.x, &game->userInputs.mouse.position.y);
    
    // Verifica se há eventos na fila de eventos do SDL.
    while (SDL_PollEvent(&game->event)) {
        switch (game->event.type) {
            // Se o evento for de fechar a janela, o jogo termina.
            case SDL_QUIT:
                game->isRunning = false;
                break;
            case SDL_KEYDOWN:
                handleKeyDown(game);
                break;
            case SDL_KEYUP:
                handleKeyUp(game);
                break;
            // Verifica eventos de clique do mouse.
            case SDL_MOUSEBUTTONDOWN:
                handleMouseButtonsDown(game);
                break;
            case SDL_MOUSEBUTTONUP:
                handleMouseButtonsUp(game);
                break;
        }
    }
}

#endif // HANDLE_EVENTS_H