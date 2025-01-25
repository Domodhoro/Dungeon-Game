#ifndef HANDLE_EVENTS_H
#define HANDLE_EVENTS_H

// Função que trata os eventos.
void handleEvents(Game *game) {
    // Verifica se há eventos na fila de eventos do SDL.
    while (SDL_PollEvent(&game->event)) {
        switch (game->event.type) {
#if DEV
        // Processa o evento tecla ESC pressionada (somente para a fase de desenvolvimento).
        case SDL_KEYDOWN:
            if (game->event.key.keysym.sym == SDLK_ESCAPE) {
                // Se a tecla pressionada for ESC, encerra o jogo.
                game->running = false;
            }
            if (game->event.key.keysym.sym == SDLK_p) {
                
            }
            break;
#endif // DEV
        // Se o evento for de fechar a janela, o jogo termina.
        case SDL_QUIT:
            game->running = false;
            break;
        // Verifica eventos de clique do mouse.
        case SDL_MOUSEBUTTONDOWN:
            if (game->event.button.button == SDL_BUTTON_LEFT) {
                // Botão esquerdo pressionado.
                game->userInputs.mouse.leftButton = true;
            } else if (game->event.button.button == SDL_BUTTON_RIGHT) {
                // Botão direito pressionado.
                game->userInputs.mouse.rightButton = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (game->event.button.button == SDL_BUTTON_LEFT) {
                // Botão esquerdo solto.
                game->userInputs.mouse.leftButton = false;
            } else if (game->event.button.button == SDL_BUTTON_RIGHT) {
                // Botão direito solto.
                game->userInputs.mouse.rightButton = false;
            }
            break;
        }
    }

    // Retorna as coordenadas atuais do cursor do mouse.
    SDL_GetMouseState(&game->userInputs.mouse.position.x, &game->userInputs.mouse.position.y);

    // Atualiza o estado atual do teclado.
    game->userInputs.keyboard.states = SDL_GetKeyboardState(NULL);
}

#endif // HANDLE_EVENTS_H