#ifndef HANDLE_EVENTS_H
#define HANDLE_EVENTS_H

// Função que trata os eventos do jogo.
void handleEvents(Game *game) {
	// Verifica se há eventos na fila de eventos do SDL.
    while (SDL_PollEvent(&game->event)) {
    	// Se o evento for de fechar a janela, o jogo termina.
        if (game->event.type == SDL_QUIT) {
            game->running = false;
        }
    }
    // Atualiza o estado atual do teclado.
    game->keyboard.states = SDL_GetKeyboardState(NULL);
    // Retorna as coordenadas atuais do cursor do mouse.
    SDL_GetMouseState(&game->mouse.position.x, &game->mouse.position.y);
}

#endif // HANDLE_EVENTS_H