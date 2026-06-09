#include "../object.h"

Object::Object(SDL_Renderer *renderer, float x, float y)
    : renderer(renderer), image(renderer, "images/object.png")
{
    Position.x = x;
    Position.y = y;
    rect.x = Position.x - image.width / 2;
    rect.y = Position.y;
    rect.w = image.width;
    rect.h = image.height;
}

void Object::render(Vector2D Camera)
{
    dst = rect;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    image.render(nullptr, &dst);
}