#include "../toggle.h"

Toggle::Toggle(SDL_Renderer *renderer, float x, float y)
    : Widget(renderer, x, y, [this]{ value = !value; }),
      uncheckedImg(renderer, "ui/toggle/unchecked.png"), 
      checkedImg(renderer, "ui/toggle/checked.png"), 
      image(uncheckedImg)
{
    rect.w = image.width;
    rect.h = image.height;
}

void Toggle::render(Vector2D Camera)
{
    image.render(nullptr, &rect);
}

void Toggle::handle(double dt)
{
    image = (value) ? checkedImg : uncheckedImg;
}

void Toggle::update(SDL_Event event)
{
    if (clicked(event))
        value = !value;
}

bool Toggle::hovered() { return checkCollision(getMousePosition(), rect); }

bool Toggle::clicked(SDL_Event event)
{
    return (event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
            event.button.button == SDL_BUTTON_LEFT && hovered());
}