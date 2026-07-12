#include "../stone.h"

Stone::Stone(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "images/stones/0.png", x, y)
{
    destructLvl = maxDestructLvl;
}

void Stone::render(Vector2D Camera)
{
    if (destructed)
        return;
    Sprite::render(Camera);
}

void Stone::handle(double dt, vector<Object> &objects)
{
    destructed = (destructLvl >= maxDestructLvl);
    if (destructed)
        return;
    Sprite::handle(dt, objects);
}

void Stone::destruct(int byPoints)
{
    destructLvl += byPoints;
}