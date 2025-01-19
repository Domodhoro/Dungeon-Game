#ifndef HANDLE_EVENTS_H
#define HANDLE_EVENTS_H

// Função que trata os eventos.
void handleEvents(Game *game) {
	// Verifica se há eventos na fila de eventos do SDL.
    while (SDL_PollEvent(&game->event)) {
        switch (game->event.type) {
        // Se o evento for de fechar a janela, o jogo termina.
        case SDL_QUIT:
            game->running = FALSE;
            break;
        // Verifica eventos de clique do mouse.
        case SDL_MOUSEBUTTONDOWN:
            if (game->event.button.button == SDL_BUTTON_LEFT) {
                // Botão esquerdo pressionado.
                game->mouse.leftButton = TRUE;
            } else if (game->event.button.button == SDL_BUTTON_RIGHT) {
                // Botão direito pressionado.
                game->mouse.rightButton = TRUE;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (game->event.button.button == SDL_BUTTON_LEFT) {
                // Botão esquerdo solto.
                game->mouse.leftButton = FALSE;
            } else if (game->event.button.button == SDL_BUTTON_RIGHT) {
                // Botão direito solto.
                game->mouse.rightButton = FALSE;
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
