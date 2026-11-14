#include "../../objects/pad.h"

Pad::Pad(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "empty.png", x, y),
        usingImg(renderer, "images/objects/pads/1.png"), 
        notUsingImg(renderer, "images/objects/pads/0.png") 
{
}

void Pad::render(Vector2D Camera)
{
    image = (inUse) ? usingImg : notUsingImg;
    Sprite::render(Camera);
}