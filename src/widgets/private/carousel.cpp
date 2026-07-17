#include "../carousel.h"

Carousel::Carousel(SDL_Renderer *renderer, float x, float y, 
    UIFunction callback, vector<string> &data)
    : Widget(renderer, x, y, callback), data(data),
      text(renderer, x, y, data.at(index), colors.white, 8),
      rightArrow(renderer, "ui/arrows/right.png"),
      leftArrow(renderer, "ui/arrows/left.png")
{
    rightRect = {x + (text.rect.w / 2), y, rightArrow.width, rightArrow.height};
    leftRect = {x - (text.rect.w / 2), y, leftArrow.width, leftArrow.height};
    text.rect.x += text.rect.w / 2;
    text.rect.y += text.rect.h / 2;
    maxIdx = data.size() - 1;
}

void Carousel::render(Vector2D Camera)
{
    text.render();
    rightArrow.render(nullptr, &rightRect);
    leftArrow.render(nullptr, &leftRect);
}

void Carousel::handle(double dt)
{
    text.updateData(data.at(index));
}

void Carousel::update(SDL_Event event)
{
    if (clicked(rightRect, event))
    {
        if (index >= maxIdx)
            index = 0;
        else
            index++;
        onCallback();
    }
    else if (clicked(leftRect, event))
    {
        if (index <= 0)
            index = maxIdx - 1;
        else
            index--;
        onCallback();
    }
}

bool Carousel::hovered(SDL_FRect rect) 
{ 
    return checkCollision(getMousePosition(), rect); 
}

bool Carousel::clicked(SDL_FRect rect, SDL_Event event)
{
    return (event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
            event.button.button == SDL_BUTTON_LEFT && hovered(rect));
}