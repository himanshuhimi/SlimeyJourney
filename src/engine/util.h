#pragma once

#include <SDL3_mixer/SDL_mixer.h>
#include "graphics.h"
#include "vector.h"
#include "defines.h"

struct Audio
{
    MIX_Mixer *mixer = nullptr;
    MIX_Audio *audio = nullptr;
    MIX_Track *track = nullptr;
    Audio(string audioSource);
    void play(int volume = 100, int times = 0);
};

struct Cooldown
{
    bool available = false;
    double duration = 0.0;
    double timeElapsed = 0.0;
    Cooldown(double duration = 2.0);
    void handle(double dt);
    void reset();
};

struct Timer
{
    SDL_Renderer *renderer = nullptr;
    Text text;
    double duration = 0.0, currentTime = 0.0;
    Timer(SDL_Renderer *renderer, float x, float y, double duration = 1.0);
    void handle(double dt);
    void render();
};

struct LineOfSight
{
    SDL_Renderer *renderer = nullptr;
    SDL_FRect rect, dst;
    Vector2D Position;
    bool visible = false;
    LineOfSight(SDL_Renderer *renderer, float x, float y, float width, float height);
    void matchPosition(SDL_FRect matchRect);
    void render(Vector2D Camera);
};
