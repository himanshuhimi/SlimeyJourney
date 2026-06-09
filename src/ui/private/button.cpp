#include "../button.h"

Button::Button(SDL_Renderer *renderer, float x, float y,
               std::function<void()> callback, string label,
                SDL_Color color)
    : renderer(renderer), callback(std::function<void()>(callback)),
      image(nullptr, ""), color(color),
      text(renderer, 0, 0, "", colors.black)
{
    Image unhovered = Image(renderer, "assets/ui/buttons/unhovered.png");
    Image hovered = Image(renderer, "assets/ui/buttons/hovered.png");
    images.emplace_back(unhovered);
    images.emplace_back(hovered);
    
    rect = SDL_FRect{x - (images[0].width / 2), y, images[0].width, images[0].height};
    
    text = Text(
        renderer, rect.x + (rect.w / 2), rect.y + (rect.h / 2),
        label, SDL_Color{33, 35, 59, 255}, 18);
    
    image = images[0];
}

void Button::handle(SDL_Event event)
{
    if (clicked(event))
        callback();
}

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

void Button::update(double dt) { image = images[hovered()]; }