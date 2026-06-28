#include "../quest.h"

Quest::Quest(SDL_Renderer *renderer, float x, float y, string label)
    : renderer(renderer), label(label), completeImage(renderer, "ui/complete.png")
{
    text = new Text(renderer, x, y, 
        label, colors.black, 10, 1,
        Image(renderer, "ui/incomplete.png"));
}

void Quest::handle(double dt) 
{
    if (completed)
        text->updateAttach(completeImage);
}

void Quest::render(Vector2D Camera) { text->render(Camera); }