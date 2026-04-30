#include "../grass.h"

Grass::Grass(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "assets/images/grass.png", x, y)
{
}