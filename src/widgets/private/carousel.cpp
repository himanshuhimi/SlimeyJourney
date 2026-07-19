#include "../carousel.h"

Carousel::Carousel(SDL_Renderer *renderer, float x, float y,
                   UIFunction callback, vector<string> &data,
                   string defaultVal)
    : Widget(renderer, x, y, callback), data(data),
      text(renderer, x, y, "", colors.white, 8),
      rightUnhovered(renderer, "ui/arrows/right/unhovered.png"),
      leftUnhovered(renderer, "ui/arrows/left/unhovered.png"),
      leftHovered(renderer, "ui/arrows/left/hovered.png"),
      rightHovered(renderer, "ui/arrows/right/hovered.png"),
      rightArrow(renderer, "ui/arrows/right/unhovered.png"),
      leftArrow(renderer, "ui/arrows/left/unhovered.png")
{
    if (defaultVal != "")
    {
        auto it = std::find(data.begin(), data.end(), defaultVal);
        index = (it != data.end()) ? (it - data.begin()) : -1;
    }
    text.updateData(data.at(index));
    maxIdx = data.size() - 1;
    maxElem = *std::max_element(
        data.begin(),
        data.end(),
        [](string &a, string &b)
        {
            return a.size() < b.size();
        });
    Text maxText(renderer, x, y, maxElem, colors.white, text.pixelSize);
    rightRect = {
        x + (maxText.rect.w), 
        y, 
        rightArrow.width, 
        rightArrow.height};
    leftRect = {
        x - (maxText.rect.w / 4), 
        y, 
        leftArrow.width, 
        leftArrow.height};
}

void Carousel::render(Vector2D Camera)
{
    text.render();
    rightArrow.render(nullptr, &rightRect);
    leftArrow.render(nullptr, &leftRect);
}

void Carousel::handle(double dt)
{
    if (hovered(rightRect))
        rightArrow = rightHovered;
    else if (hovered(leftRect))
        leftArrow = leftHovered;
    else
    {
        rightArrow = rightUnhovered;
        leftArrow = leftUnhovered;
    }
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