#pragma once

#define SDL_MAIN_HANDLED

#include <iostream>
#include <vector>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

using std::string, std::vector;

extern const string TITLE;
extern const int SPRITE_SIZE;
extern int WIDTH, HEIGHT;

void log(string message);
void log(int number);

struct Vector2D
{
    float x, y;
    Vector2D(float x = 0.0f, float y = 0.0f) : x{x}, y{y} {};
};
struct Image
{
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *texture = nullptr;
    Image(SDL_Renderer *renderer, string source);
    void render(const SDL_FRect *srcrect, const SDL_FRect *dstrect);
};
struct Text
{
    SDL_Renderer *renderer = nullptr;
    SDL_Surface *surface = nullptr;
    SDL_Texture *texture = nullptr;
    TTF_Font *font = nullptr;
    SDL_FRect rect;
    SDL_Color color;
    string data, fontSource;
    float x, y;
    int pixelSize, alpha;
    Text(
        SDL_Renderer *renderer,
        float x,
        float y,
        string data,
        string fontSource,
        SDL_Color color,
        int pixelSize = 24
    );
    void render();
    void updateData(string newData);
    void updateAlpha(int newAlpha);
};

enum constants
{
    gravity = 980
};