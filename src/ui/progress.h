#pragma once

#include "../core/config.h"

class Progress
{
public:
    SDL_Renderer *renderer = nullptr;
    Image barImage;
    SDL_FRect rect, fillRect, dst;
    SDL_Color color;
    double startPercent = 0.0, animSpeed = 0.0, reachPercent = 0.0, percentage = 0.0;
    Progress(
        SDL_Renderer *renderer,
        float x,
        float y,
        SDL_Color color = colors.white,
        float width = 100,
        double startPercent = 0.0,
        double animSpeed = 2.0
    );
    void update(double increment = 0.2);
    void handle(double dt);
    void render(Vector2D Camera = Vector2D{0.0f, 0.0f});
};