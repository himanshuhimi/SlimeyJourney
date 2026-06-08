#include "../button.h"

Button::Button(SDL_Renderer *renderer, float x, float y,
               std::function<void()> callback, string label)
    : renderer(renderer), callback(std::function<void()>(callback)),
      image(renderer, "assets/ui/buttons/unhovered.png"),
      text(renderer, 0, 0, "", colors.black)
{
    rect = SDL_FRect{x - (image.width / 2), y, image.width, image.height};
    text = Text(
        renderer, rect.x + (rect.w / 2), rect.y + (rect.h / 2),
        label, SDL_Color{33, 35, 59, 255}, 18);
    images.emplace_back(renderer, "assets/ui/buttons/unhovered.png");
    images.emplace_back(renderer, "assets/ui/buttons/hovered.png");
}

void Button::handle(SDL_Event event)
{
    if (clicked(event))
        callback();
}

void Button::render()
{
    image.render(nullptr, &rect);
    text.render();
}

bool Button::hovered() { return checkCollision(getMousePosition(), rect); }

bool Button::clicked(SDL_Event event)
{
    return (event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
            event.button.button == SDL_BUTTON_LEFT && hovered());
}

void Button::update(double dt) { image = images[hovered()]; }