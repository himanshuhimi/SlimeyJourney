#include "../../systems/quest.h"

Quest::Quest(SDL_Renderer *renderer, float x, float y, string label)
    : renderer(renderer), label(label), completeImage(renderer, "ui/complete.png"),
      bg(renderer, "ui/quest.png")
{
    text = new Text(renderer, x, y, 
        label, colors.black, 10, 1,
        Image(renderer, "ui/incomplete.png"));
    // bgRect.w = bg.width;
    // bgRect.h = bg.height;
    // bgRect.x = text->attachRect.x - (text->attachRect.w / 2);
    // bgRect.y = text->attachRect.y;
}

void Quest::handle(double dt) 
{
    if (completed)
        text->updateAttach(completeImage);
}

void Quest::render(Vector2D Camera) 
{
    // bg.render(nullptr, &bgRect);
    text->render(Camera); 
}