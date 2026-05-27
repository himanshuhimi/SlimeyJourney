#include "../fruit.h"

Fruit::Fruit(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "empty.png", x, y) 
{
    vector<string> fruits = {"apple", "melon", "orange"};
    string choice = Random.choice<string>(fruits);
    string path = "assets/images/fruits/" + choice + ".png";
    image = new Image(renderer, path);
    percentage = juiceStrengths.at(choice);
}

void Fruit::render(Vector2D Camera)
{
    if (taken)
        return;
    Sprite::render(Camera);
}