#pragma once

#include "core/game.h"
#include "widgets/progress.h"
#include "widgets/button.h"
#include "widgets/card.h"

using std::pair;

class Game;

struct UIScreen
{
    using Widgets = vector<pair<string, unique_ptr<Widget>>>;
    using CategoryWid = map<string, Widgets>;
    CategoryWid ctgWidgets = {};
    UIScreen(Game &game);
    virtual void render(Vector2D Camera = {});
    virtual void handle(double dt);
    virtual void update(SDL_Event event);
    template <class T>
    T &getWidget(string category, string name)
    {
        auto &ctg = ctgWidgets.at(category);
        for (auto &[label, widget] : ctg)
            if (label == name)
                return dynamic_cast<T &>(*widget);
        throw std::runtime_error("Widget not found: " + category + ": " + name);
    }

protected:
    Game &game;
};

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

protected:
    Game &game;
};

struct HomeScreen : public UIScreen
{
    Image titleImage;
    SDL_FRect titleRect;
    HomeScreen(Game &game);
    void render(Vector2D Camera = {}) override;
};

struct LoadingScreen : public UIScreen
{
    LoadingScreen(Game &game);
    void handle(double dt) override;
};

struct SettingsScreen : public UIScreen
{
    SettingsScreen(Game &game);
};

struct SelectionScreen : public UIScreen
{
    SelectionScreen(Game &game);
};

struct PlayingScreen : public UIScreen
{
    Hearts hearts;
    PlayingScreen(Game &game);
    void render(Vector2D Camera = {}) override;
    void handle(double dt) override;
};

struct PausedScreen : public UIScreen
{
    Text title;
    PausedScreen(Game &game);
    void render(Vector2D Camera = {}) override;
};

struct OverScreen : public UIScreen
{
    Text title;
    OverScreen(Game &game);
    void render(Vector2D Camera = {}) override;
};

class UI
{
public:
    map<Scenes, unique_ptr<UIScreen>> screens = {};
    UIScreen *activeScreen = nullptr;
    UI(Game &game);
    void render(Vector2D Camera = {});
    void handle(double dt);
    void update(SDL_Event event);
    void updateScreen(Scenes scene);
    template <class T>
    T &getWidget(string category, string name)
    {
        return activeScreen->getWidget<T>(category, name);
    }

private:
    Game &game;
};