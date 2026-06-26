#include "../heart.h"

Heart::Heart(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "hearts/normal.png", x, y) 
{
    images.emplace_back(renderer, "images/hearts/broken.png");
    images.emplace_back(renderer, "images/hearts/normal.png");
}

void Heart::handle(double dt, vector<Object> &objects)
{
    image = images[(int)!broken];
}

void Heart::render(Vector2D Camera)
{
    Sprite::render({0, 0});
}