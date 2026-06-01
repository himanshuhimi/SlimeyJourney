#pragma once

#define SDL_MAIN_HANDLED

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <filesystem>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <tinyxml2.h>

using std::string, std::vector, std::unordered_map;

extern const string TITLE;
extern const float SPRITE_SIZE;
extern int WIDTH, HEIGHT;
extern const int CAMERA_X, CAMERA_Y;
extern const unordered_map<string, double> juiceStrengths;

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
    float width = 0, height = 0;
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
        SDL_Color color,
        int pixelSize = 24,
        string fontSource = "assets/fonts/roboto.ttf");
    void render();
    void updateData(string newData);
    void updateAlpha(int newAlpha);
};
struct Animation
{
    SDL_Renderer *renderer = nullptr;
    Image *imageSet = nullptr;
    SDL_FRect src, dst;
    int index = 0;
    float timer = 0.0f, frameTime = 0.5f;
    Animation(SDL_Renderer *renderer, SDL_FRect dst, string source);
    void handle(double dt);
    void render(Vector2D Camera);
};
struct Audio
{
    MIX_Mixer *mixer = nullptr;
    MIX_Audio *audio = nullptr;
    MIX_Track *track = nullptr;
    Audio(string audioSource);
    void play(int times = 0);
};
struct Cooldown
{
    bool available = false;
    double duration = 0.0;
    double timeElapsed = 0.0;
    Cooldown(double duration = 2.0);
    void handle(double dt);
};
extern struct _Random_
{
    int randint(int begin, int end);
    template <typename T>
    T choice(vector<T> vec)
    {
        if (vec.empty())
            log("Empty Vector.");
        return vec.at(randint(0, vec.size() - 1));
    }
} Random;
extern struct _Colors_
{
    SDL_Color white = SDL_Color{255, 255, 255, 255};
    SDL_Color black = SDL_Color{0, 0, 0, 255};
    SDL_Color red = SDL_Color{255, 0, 0, 255};
    SDL_Color green = SDL_Color{0, 255, 0, 255};
    SDL_Color blue = SDL_Color{0, 0, 255, 255};
    SDL_Color orange = SDL_Color{255, 165, 0, 255};
    SDL_Color yellow = SDL_Color{255, 206, 27, 255};
    SDL_Color pink = SDL_Color{255, 182, 193, 255};
    SDL_Color violet = SDL_Color{27, 76, 255, 255};
    _Colors_() {};
    SDL_Color hexToRgb();
} colors;
extern struct _Consts_
{
    double gravity = 245;
} constants;