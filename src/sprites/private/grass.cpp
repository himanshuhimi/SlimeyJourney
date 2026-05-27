#include "../grass.h"

Grass::Grass(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "empty.png", x, y) {}