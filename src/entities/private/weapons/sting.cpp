#include "../../weapons/sting.h"

Sting::Sting(SDL_Renderer *renderer, float x, float y, Vector2D Direction)
    : Weapon(renderer, "weapons/sting.png", x, y, Direction) {}