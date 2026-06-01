#include "../grass.h"

Grass::Grass(SDL_Renderer *renderer, float x, float y)
    : renderer(renderer), image(renderer, "assets/images/empty.png")
{
    
    Position.x = x;
    Position.y = y;
    rect.w = image.width;
    rect.h = image.height;
    rect.x = Position.x - rect.w / 2;
    rect.y = Position.y;
}

void Grass::render(Vector2D Camera)
{
    dst = rect;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    image.render(nullptr, &dst);
}