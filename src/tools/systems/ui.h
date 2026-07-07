#pragma once

#include "core/game.h"
#include "widgets/progress.h"
#include "widgets/button.h"

using UIFunction = std::function<void()>;
using std::pair;

class Game;

struct Hearts
{
    Image normalHeart, brokenHeart;
    vector<SDL_FRect> rects = {};
    vector<Image> images = {};
    Hearts(Game &game);
    void render();
    void handle();
    void clear();
    void load();

private:
    Game &game;
};

class UI
{
public:
    Hearts hearts;
    SDL_Renderer *renderer = nullptr;
    map<string, Progress> progresses = {};
    map<string, Button> buttons = {};
    map<Image *, SDL_FRect> images = {};
    vector<Text> texts = {};
    UI(Game &game);
    void handle(double dt);
    void render();
    void update(SDL_Event event);

private:
    Game &game;
    void load();
    void loadProgresses();
    void loadButtons();
    void loadImages();
    void loadTexts();
    vector<string> getBtnNames(States state);
    vector<string> getProgNames(States state);
    vector<string> getTitleNames();
    vector<pair<string, UIFunction>> getBtnFuncs();
    vector<pair<string, UIFunction>> getProgFuncs();
    template <typename T>
    const T &getByName(const vector<pair<string, T>> &vec, const string &name)
    {
        auto it = std::find_if(
            vec.begin(),
            vec.end(),
            [&](const auto &p)
            { return p.first == name; });
        if (it == vec.end())
            throw std::out_of_range("Key not found");
        return it->second;
    }
};