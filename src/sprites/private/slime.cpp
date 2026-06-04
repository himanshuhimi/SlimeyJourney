#include "../slime.h"

Slime::Slime(SDL_Renderer *renderer, float x, float y)
    : Enemy(renderer, "slime", x, y, Random.randint(5, 15)) {}