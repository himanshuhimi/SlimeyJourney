#include "../fruit.h"

Fruit::Fruit(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "object.png", x, y)
{
    choice = Random.choice<string>(fruitTypes);
    image = Image(renderer, "images/fruits/" + choice + ".png");
}

void Fruit::handle(double dt, const vector<Object> &objects)
{
    if (picked)
        return;
    Sprite::handle(dt, objects);
}

void Fruit::render(Vector2D Camera)
{
    if (picked)
        return;
    Sprite::render(Camera);
}