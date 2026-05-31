#include "../grass.h"

Grass::Grass(SDL_Renderer *renderer, float x, float y)
    : renderer(renderer), image(renderer, "assets/images/empty.png")
{
    
    Position.x = x - rect.w / 2;
    Position.y = y;
    rect.x = Position.x;
    rect.y = Position.y;
}

void Grass::render(Vector2D Camera)
{
    dst = rect;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    image.render(nullptr, &dst);
}