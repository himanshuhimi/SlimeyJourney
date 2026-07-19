#include "../card.h"

Card::Card(
    SDL_Renderer *renderer,
    float x,
    float y,
    string label,
    UIFunction callback)
    : Widget(renderer, x, y, callback), label(label),
      bgUnhovered(renderer, "ui/card/unhovered.png"),
      bgHovered(renderer, "ui/card/hovered.png"),
      displayText(renderer, 0, 0, label, colors.white)
{
    rect.w = bgUnhovered.width;
    rect.h = bgUnhovered.height;
    rect.x -= rect.w / 2;
    rect.y -= rect.h / 2;
    displayText.rect.x = x - displayText.rect.w / 2;
    displayText.rect.y = y - displayText.rect.h / 2;
}

void Card::handle(double dt) {}

void Card::render(Vector2D Camera)
{
    if (hovered())
        bgHovered.render(nullptr, &rect);
    else
        bgUnhovered.render(nullptr, &rect);
    displayText.render(Camera);
}

bool Card::hovered() { return checkCollision(getMousePosition(), rect); }

bool Card::clicked(SDL_Event event)
{
    return (event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
            event.button.button == SDL_BUTTON_LEFT && hovered());
}

void Card::update(SDL_Event event)
{
    if (clicked(event))
        onCallback();
}