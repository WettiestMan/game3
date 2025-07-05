#include "include/Game.hpp"  

int main() {
    Game* game = Game::get_instance();
    return game->run();
}
