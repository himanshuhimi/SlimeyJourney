#include "../button.h"

Button::Button(SDL_Renderer *renderer, float x, float y,
               std::function<void()> callback, string label,
                SDL_Color color)
    : renderer(renderer), callback(std::function<void()>(callback)),
      image(nullptr, ""), color(color), label(label),
      text(renderer, 0, 0, "", colors.black)
{
    image = Image(renderer, "ui/button.png");
    rect = SDL_FRect{x - (image.width / 2), y, image.width, image.height};
    text = Text(
        renderer, rect.x + (rect.w / 2), rect.y + (rect.h / 2),
        label, SDL_Color{33, 35, 59, 255}, 18);
}

void Button::handle(double dt) {}

void Button::render()
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
    image.render(nullptr, &rect);
    text.render();
}

bool Button::hovered() { return checkCollision(getMousePosition(), rect); }

bool Button::clicked(SDL_Event event)
{
    return (event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
            event.button.button == SDL_BUTTON_LEFT && hovered());
}

void Button::update(SDL_Event event)
{
    if (clicked(event))
        callback();
}