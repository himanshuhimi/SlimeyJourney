#include "../LOS.h"

LOS::LOS(SDL_Renderer *renderer, float x, float y, float width, float height)
    : renderer(renderer)
{
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
}

void LOS::matchPosition(SDL_FRect matchRect)
{
    rect.x = matchRect.x + (matchRect.w / 2);
    rect.y = matchRect.y + (matchRect.h / 2);
}

void LOS::render(Vector2D Camera)
{
    dst = rect;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &dst);
}