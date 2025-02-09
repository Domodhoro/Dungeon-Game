#ifndef SETUP_H
#define SETUP_H

// Função responsável por configurar os objetos do jogo.
_Bool setup(Game *game) {    
    // Configura os objetos do jogo.
    setupCamera(game);
    setupPlayer(game);
    setupInventory(game);
    if (not setupDungeon(game) or not setupMainMenu(game)) {
        return false;
    }
    return true;
}

#endif // SETUP_H