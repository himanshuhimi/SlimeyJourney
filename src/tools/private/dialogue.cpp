#include "../dialogue.h"

Dialogue::Dialogue(
    SDL_Renderer *renderer,
    float x, float y,
    string initialText)
    : renderer(renderer), box(renderer, "ui/dialogue.png")
{
    Position = {x, y};
    rect = {x, y, box.width, box.height};
    rect.x -= rect.w / 2;
    rect.y -= rect.h / 2;
    text = new Text(
        renderer, x, y,
        initialText, colors.white,
        10, 0);
}

void Dialogue::handle(double dt) {}

void Dialogue::render(Vector2D Camera) 
{
    SDL_FRect dst = rect;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    box.render(nullptr, &dst);
    text->render(Camera);
}

void Dialogue::updateText(string newData) { text->updateData(newData); }