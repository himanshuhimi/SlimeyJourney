#include "../../passives/checkpoint.h"

Checkpoint::Checkpoint(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "empty.png", x, y) 
{
    dialogue = new Dialogue(renderer, x, y - SPRITE_SIZE, "Need.. food!");
}

void Checkpoint::handle(double dt, vector<Object> &objects)
{
    dialogue->Position = Position;
    dialogue->handle(dt);
    if (helped)
        dialogue->updateText("Thank you so much..!! Dear Friend!");
    Sprite::handle(dt, objects);
    dialogue->handle(dt);
}

void Checkpoint::render(Vector2D Camera)
{
    Sprite::render(Camera);
    dialogue->render(Camera);
}