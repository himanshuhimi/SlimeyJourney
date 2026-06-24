#pragma once

#include "core/config.h"

class Progress
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_FRect attachmentRect, rect, fillRect, dst;
    Image attachment, image;
    SDL_Color color;
    Vector2D Position;
    double startPercent = 0.0, animSpeed = 0.0, reachPercent = 0.0, percentage = 0.0;
    bool complete = false;
    Progress(
        SDL_Renderer *renderer, float x, float y, 
        SDL_Color color = SDL_Color{255, 255, 255, 255},
        Image attachment = {nullptr, ""}, double startPercent = 0.0,
        float width = 100, double animSpeed = 2.0);
    void update(double increment = 0.2);
    void handle(double dt);
    void render(Vector2D Camera = Vector2D{0.0f, 0.0f});
    void reset();
};