#pragma once

#define SDL_MAIN_HANDLED

#include <iostream>
#include <algorithm>
#include <filesystem>
#include <functional>
#include <random>
#include <vector>
#include <cmath>
#include <map>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <tinyxml2.h>

using std::string, std::vector, std::map;
namespace fs = std::filesystem;

extern const string TITLE;
extern const float SPRITE_SIZE;
extern int WIDTH, HEIGHT;
extern int CHANGED_WIDTH, CHANGED_HEIGHT;
extern int scaleX, scaleY;
extern int CAMERA_X, CAMERA_Y;
extern bool scaled;
extern const vector<string> fruits;
extern SDL_RendererLogicalPresentation logicalPresentation;

template <typename T>
void print(const T &message) { std::cout << "[LOG] " << message << std::endl; };
bool checkCollision(SDL_FRect A, SDL_FRect B);
SDL_FRect getMousePosition();

struct Vector2D
{
    float x, y;
    Vector2D(float x = 0.0f, float y = 0.0f);
    float distanceFromVec(Vector2D secVector);
    float getLength();
    Vector2D normalise();
};

struct Image
{
    SDL_Renderer *renderer = nullptr;
    SDL_Surface *surface = nullptr;
    SDL_Texture *texture = nullptr;
    float width = 0, height = 0;
    string source = "";
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
    Text(SDL_Renderer *renderer, float x, float y,
         string data, SDL_Color color, int pixelSize = 24,
         string fontSource = "fonts/pixel.ttf");
    void render();
    void updateData(string newData);
    void updateAlpha(int newAlpha);
};

struct Animation
{
    SDL_Renderer *renderer = nullptr;
    SDL_FRect src, renderDst, dst;
    Image imageSet;
    int maxFrames = 0.0, index = 0;
    bool active = false, complete = false;
    float timer = 0.0f, frameTime = 0.5f;
    Animation(SDL_Renderer *renderer, string source, float frameTime = 0.5f);
    void handle(double dt);
    void render(Vector2D Camera, SDL_FRect dst);
    void restart();
};

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
};
struct LineOfSight
{
    SDL_Renderer *renderer = nullptr;
    SDL_FRect rect, dst;
    Vector2D Position;
    LineOfSight(SDL_Renderer *renderer, float x, float y, float width, float height);
    void matchPosition(SDL_FRect matchRect);
    void render(Vector2D Camera);
};

struct Timer
{
    SDL_Renderer *renderer = nullptr;
    Text text;
    double duration = 0.0, currentTime = 0.0;
    Timer(SDL_Renderer *renderer, double duration = 1.0);
    void handle(double dt);
    void render();
};

extern struct _Random_
{
    int randint(int begin, int end);
    template <typename T>
    T choice(vector<T> vec)
    {
        if (vec.empty())
            print("Empty Vector.");
        return vec.at(randint(0, vec.size() - 1));
    }
} Random;

extern struct _Colors_
{
    SDL_Color white = SDL_Color{255, 255, 255, 255};
    SDL_Color black = SDL_Color{0, 0, 0, 255};
    SDL_Color red = SDL_Color{179, 0, 0, 255};
    SDL_Color green = SDL_Color{0, 255, 0, 255};
    SDL_Color blue = SDL_Color{0, 0, 255, 255};
    SDL_Color skyblue = SDL_Color{41, 160, 204, 255};
    SDL_Color orange = SDL_Color{255, 165, 0, 255};
    SDL_Color yellow = SDL_Color{255, 206, 27, 255};
    SDL_Color pink = SDL_Color{255, 182, 193, 255};
    SDL_Color violet = SDL_Color{27, 76, 255, 255};
    _Colors_() {};
    SDL_Color hexToRgb(string color)
    {
        color.erase(0, color.find_first_not_of('#'));
        unsigned char r = std::stoi(color.substr(0, 2), nullptr, 16);
        unsigned char g = std::stoi(color.substr(2, 2), nullptr, 16);
        unsigned char b = std::stoi(color.substr(4, 2), nullptr, 16);
        return SDL_Color{r, g, b, 255};
    }
} colors;

extern struct _Consts_
{
    double gravity = 245;
} constants;

enum States
{
    HOME,
    LOADING,
    SETTINGS,
    PLAYING,
    PAUSED,
    PROGRESSING,
    OVER
};

extern map<int, string> titles;
extern map<int, SDL_Color> titleColors;
extern map<int, double> durations;