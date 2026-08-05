#include "../../weapons/spit.h"

Spit::Spit(SDL_Renderer *renderer, float x, float y, Vector2D Direction)
    : Weapon(renderer, "spits/0.png", x, y, Direction) {}