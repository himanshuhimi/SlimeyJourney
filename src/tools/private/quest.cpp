#include "../quest.h"

Quest::Quest(SDL_Renderer *renderer, float x, float y, string label)
    : renderer(renderer), label(label) 
{
    text = new Text(renderer, x, y, label, colors.black, 12);
}

void Quest::handle(double dt) {}

void Quest::render(Vector2D Camera)
{
    text->render(Camera);
}