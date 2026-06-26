#include "../fren.h"

Fren::Fren(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "fren/idle.png", x, y) 
{
    dialogue = new Dialogue(renderer, x, y - SPRITE_SIZE, "Need.. food!");
}

void Fren::handle(double dt, vector<Object> &objects)
{
    dialogue->Position = Position;
    dialogue->handle(dt);
    if (helped)
        dialogue->updateText("Thank you so much..!! Dear Friend!");
    Sprite::handle(dt, objects);
    dialogue->handle(dt);
}

void Fren::render(Vector2D Camera)
{
    Sprite::render(Camera);
    dialogue->render(Camera);
}