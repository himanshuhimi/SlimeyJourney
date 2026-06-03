#include "../slime.h"

Slime::Slime(SDL_Renderer *renderer, float x, float y)
    : Enemy(renderer, "slimes", x, y, 10) {}