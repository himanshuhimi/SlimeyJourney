#include "../fruit.h"

Fruit::Fruit(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "empty.png", x, y) 
{
    vector<string> fruits = {"apple", "melon", "orange"};
    string choice = Random.choice<string>(fruits);
    string path = "assets/images/fruits/" + choice + ".png";
    image = Image(renderer, path);
}

void Fruit::render(Vector2D Camera)
{
    if (picked)
        return;
    Sprite::render(Camera);
}