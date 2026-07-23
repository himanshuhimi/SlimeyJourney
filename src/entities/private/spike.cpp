#include "../spike.h"

Spike::Spike(SDL_Renderer *renderer, float x, float y, float width, float height)
    : Sprite(renderer, "object.png", x, y) 
{
    rect.w = width;
    rect.h = height;
}