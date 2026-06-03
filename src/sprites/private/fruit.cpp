#include "../fruit.h"

Fruit::Fruit(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "empty.png", x, y)
{
    choice = Random.choice<string>(fruits);
    image = Image(renderer, "assets/images/fruits/" + choice + ".png");
}

void Fruit::render(Vector2D Camera)
{
    if (picked)
        return;
    Sprite::render(Camera);
}