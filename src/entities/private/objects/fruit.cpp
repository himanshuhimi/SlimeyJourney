#include "../../objects/fruit.h"

Fruit::Fruit(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "object.png", x, y)
{
    vector<string> fruits = {};
    for (auto &entry : fs::directory_iterator("data/assets/images/fruits/")) 
        fruits.emplace_back(entry.path().stem().string());
    choice = Random.choice<string>(fruits);
    image = Image(renderer, "images/fruits/" + choice + ".png");
}

void Fruit::handle(double dt, const vector<Object> &objects)
{
    if (picked)
        return;
    time += dt;
    Velocity.y = sin(360 * time);
    Sprite::handle(dt, objects);
}

void Fruit::render(Vector2D Camera)
{
    if (picked)
        return;
    Sprite::render(Camera);
}