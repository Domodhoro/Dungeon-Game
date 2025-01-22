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
                game->mouse.leftButton = true;
            } else if (game->event.button.button == SDL_BUTTON_RIGHT) {
                // Botão direito pressionado.
                game->mouse.rightButton = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (game->event.button.button == SDL_BUTTON_LEFT) {
                // Botão esquerdo solto.
                game->mouse.leftButton = false;
            } else if (game->event.button.button == SDL_BUTTON_RIGHT) {
                // Botão direito solto.
                game->mouse.rightButton = false;
            }
            break;
        }
    }

    // Retorna as coordenadas atuais do cursor do mouse.
    SDL_GetMouseState(&game->mouse.position.x, &game->mouse.position.y);

    // Atualiza o estado atual do teclado.
    game->keyboard.states = SDL_GetKeyboardState(NULL);
}

#endif // HANDLE_EVENTS_H