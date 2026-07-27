#include "../../weapons/ball.h"

Ball::Ball(SDL_Renderer *renderer, float x, float y, string type, Vector2D Direction)
    : Weapon(renderer, type + "/ball.png", x, y, Direction) {}