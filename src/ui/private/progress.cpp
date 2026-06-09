#include "../progress.h"

Progress::Progress(
    SDL_Renderer *renderer, float x, float y, SDL_Color color,
    Image attachment, double startPercent, float width, double animSpeed)
    : renderer(renderer), color(color), attachment(attachment),
      startPercent(startPercent), animSpeed(animSpeed),
      image(renderer, "ui/bar.png")
{
    Position.x = x;
    Position.y = y;
    rect.x = (attachment.width) + Position.x;
    rect.y = Position.y;
    rect.w = width;
    rect.h = image.height;
    fillRect.x = rect.x;
    fillRect.y = rect.y;
    fillRect.h = rect.h;
    reachPercent = startPercent;
    if (attachment.renderer)
    {
        attachmentRect.w = attachment.width;
        attachmentRect.h = attachment.height;
        attachmentRect.x = rect.x - (attachmentRect.w / 2);
        attachmentRect.y = rect.y - (attachmentRect.h / 2);
    }
}

void Progress::handle(double dt)
{
    if (percentage < reachPercent)
    {
        percentage += abs(animSpeed) * dt;
        if (percentage > reachPercent)
            percentage = reachPercent;
    }
    else if (percentage > reachPercent)
    {
        percentage -= abs(animSpeed) * dt;
        if (percentage < reachPercent)
            percentage = reachPercent;
    }
    fillRect.x = dst.x;
    fillRect.y = dst.y;
    complete = (percentage >= 1.0);
}

void Progress::render(Vector2D Camera)
{
    dst = rect;
    fillRect.w = rect.w * (float)percentage;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &fillRect);
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    if (attachment.renderer)
    {
        attachmentRect.x = dst.x - attachment.width;
        attachmentRect.y = dst.y - (attachment.height / 2);
        attachment.render(nullptr, &attachmentRect);
    }
    image.render(nullptr, &dst);
}

void Progress::update(double increment)
{
    reachPercent += increment;
    if (reachPercent > 1.0)
        reachPercent = 1.0;
    else if (reachPercent < 0.0)
        reachPercent = 0.0;
    animSpeed = abs(animSpeed) * ((increment >= 0) ? 1 : -1);
}