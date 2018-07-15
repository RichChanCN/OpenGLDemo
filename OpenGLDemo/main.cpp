#include"game.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
int main(){
    Game* game = Game::Instance();
    game->gameloop();
}