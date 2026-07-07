#include "../button.h"

Button::Button(SDL_Renderer *renderer, float x, float y,
               std::function<void()> callback, string label,
               SDL_Color color)
    : Widget(renderer, x, y, callback, "button"), label(label), color(color),
        text(renderer, 0, 0, "", colors.white)
{
    rect = SDL_FRect{x, y, SPRITE_SIZE * 4, SPRITE_SIZE};
    rect.x -= rect.w / 2;
    rect.y -= rect.h / 2;
    text = Text(renderer, rect.x + (rect.w / 2), rect.y + (rect.h / 2),
                label, SDL_Color{33, 35, 59, 255}, 18);
}

void Button::handle(double dt)
{
    if (hovered())
    {
        if (scaleX < maxScale)
            scaleX += dt;
        if (scaleY < maxScale)
            scaleY += dt;
    }
    else
    {
        if (scaleX > 1)
            scaleX -= dt;
        if (scaleY > 1)
            scaleY -= dt;
    }
}

void Button::render(Vector2D Camera)
{
    dst = rect;
    dst.w *= scaleX;
    dst.h *= scaleY;
    dst.x = rect.x + (rect.w - dst.w) / 2;
    dst.y = rect.y + (rect.h - dst.h) / 2;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    shadow = dst;
    shadow.x += dst.w / 12;
    shadow.y += dst.h / 12;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, (Uint8)255/2);
    SDL_RenderFillRect(renderer, &shadow);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &dst);
    if (hovered())
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    else
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &dst);
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
        onCallback();
}