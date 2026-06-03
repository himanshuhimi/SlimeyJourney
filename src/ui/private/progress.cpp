#include "../progress.h"

Progress::Progress(
    SDL_Renderer *renderer,
    float x,
    float y,
    SDL_Color color,
    float width,
    double startPercent,
    double animSpeed) : renderer(renderer),
                       color(color),
                       startPercent(startPercent),
                       animSpeed(animSpeed),
                       barImage(renderer, "assets/images/ui/bar.png")
{
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = barImage.height;
    fillRect.x = rect.x;
    fillRect.y = rect.y;
    fillRect.h = rect.h;
    reachPercent = startPercent;
}

void Progress::update(double increment)
{
    reachPercent += increment;
    if (reachPercent > 1.0)
        reachPercent = 1.0;
    else if (reachPercent < 0.0)
        reachPercent = 0.0;
    animSpeed = std::abs(animSpeed) * ((increment >= 0) ? 1 : -1);
}

void Progress::handle(double dt)
{
    if (percentage < reachPercent)
    {
        percentage += std::abs(animSpeed) * dt;
        if (percentage > reachPercent)
            percentage = reachPercent;
    }
    else if (percentage > reachPercent)
    {
        percentage -= std::abs(animSpeed) * dt;
        if (percentage < reachPercent)
            percentage = reachPercent;
    }
    fillRect.x = dst.x;
    fillRect.y = dst.y;
}

void Progress::render(Vector2D Camera)
{
    fillRect.w = rect.w * (float)percentage;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &fillRect);
    dst = rect;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    barImage.render(nullptr, &dst);
}