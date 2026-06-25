#include "../dialogue.h"

Dialogue::Dialogue(
    SDL_Renderer *renderer,
    float x, float y,
    string initialText)
    : renderer(renderer), box(renderer, "ui/dialogue.png")
{
    Position = {x, y};
    rect = {x, y, box.width, box.height};
    text = new Text(
        renderer, rect.x - (rect.w / 2), rect.y - (rect.h / 2),
        initialText, colors.white,
        10, "fonts/roboto.ttf");
}

void Dialogue::handle(double dt)
{
    // rect.x = Position.x;
    // rect.y = Position.y;
    // text->rect.x = Position.x;
    // text->rect.y = Position.y;
}

void Dialogue::render(Vector2D Camera) 
{
    box.render(nullptr, &rect);
    SDL_FRect dst = rect;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    text->render();
}

void Dialogue::updateText(string newData) { text->updateData(newData); }