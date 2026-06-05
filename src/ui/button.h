#pragma once

#include "../core/config.h"

class Button
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_FRect rect;
    Image image;
    Text text;
    vector<Image> images = {};
    std::function<void()> callback;
    string label;
    Button(
        SDL_Renderer *renderer, float x, float y,
        std::function<void()> callback, string label);
    void handle(SDL_Event event);
    void render();
    bool hovered();
    bool clicked(SDL_Event event);
    void update(double dt);
};