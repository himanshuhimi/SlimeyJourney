#include "../ball.h"

Ball::Ball(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "ball.png", x, y) { Velocity.x = speed; }

void Ball::handle(double dt, const vector<Grass> &grasses)
{
    Sprite::handle(dt, grasses);
}