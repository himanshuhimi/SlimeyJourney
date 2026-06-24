#pragma once

#include "core/config.h"
#include "sprites/heart.h"
#include "ui/progress.h"
#include "ui/button.h"

template <typename T>
class UI
{
public:
    SDL_Renderer *renderer = nullptr;
    vector<Image *> hearts;
    Image *normalHeart, *brokenHeart;
    map<string, Progress> progresses = {};
    vector<Button> buttons = {};
    T game;
    UI(T game) : game(game), renderer(game->renderer)
    {
        normalHeart = new Image{renderer, "images/hearts/normal.png"};
        brokenHeart = new Image{renderer, "images/hearts/broken.png"};
        loadProgresses();
        loadButtons();
    }
    void handle(double dt)
    {
        switch (game->state)
        {
        case States::LOADING:
            progresses.at("loading").handle(dt);
            break;
        case States::PLAYING:
        {
            hearts.clear();
            if (hearts.size() < game->currentLevel->player.maxHP)
                for (int i = 0; i < game->currentLevel->player.maxHP; i++)
                    hearts.emplace_back(normalHeart);
            if (game->currentLevel->player.HP != game->currentLevel->player.maxHP)
                hearts.at(game->currentLevel->player.HP) = brokenHeart;
            break;
        }
        }
        for (auto &[name, progress] : progresses)
        {
            vector<string> curProgs = getProgNames();
            if (std::find(curProgs.begin(), curProgs.end(), name) != curProgs.end())
                progress.handle(dt);
        }
        for (auto &button : buttons)
        {
            vector<string> curBtns = getBtnNames();
            if (std::find(curBtns.begin(), curBtns.end(), button.label) != curBtns.end())
                button.handle(dt);
        }
    }
    void render()
    {
        switch (game->state)
        {
        case States::LOADING:
            progresses.at("loading").render();
            break;
        case States::PLAYING:
            for (int i = 0; i < hearts.size(); i++)
            {
                float startX = (WIDTH / 2 -
                                (game->currentLevel->player.maxHP / 2 * SPRITE_SIZE));
                hearts.at(i)->render(
                    nullptr,
                    new SDL_FRect{
                        startX + (i * SPRITE_SIZE),
                        HEIGHT - SPRITE_SIZE,
                        hearts.at(i)->width,
                        hearts.at(i)->height});
            }
            break;
        }
        for (auto &[name, progress] : progresses)
        {
            vector<string> curProgs = getProgNames();
            if (std::find(curProgs.begin(), curProgs.end(), name) != curProgs.end())
                progress.render();
        }
        for (auto &button : buttons)
        {
            vector<string> curBtns = getBtnNames();
            if (std::find(curBtns.begin(), curBtns.end(), button.label) != curBtns.end())
                button.render();
        }
        if (game->state == States::LOADING)
            progresses.at("loading").update();
    }
    void update(SDL_Event event)
    {
        for (auto &button : buttons)
        {
            vector<string> curBtns = getBtnNames();
            if (std::find(curBtns.begin(), curBtns.end(), button.label) != curBtns.end())
                button.update(event);
        }
    }
    void loadProgresses()
    {
        progresses.clear();
        Progress fBar = Progress(renderer, WIDTH - SPRITE_SIZE, HEIGHT / 16.0f,
                                 SDL_Color{95, 90, 204, 255},
                                 Image(renderer, "ui/bottle.png"));
        fBar.rect.x -= fBar.rect.w + fBar.attachmentRect.w;
        progresses = {
            {"loading",
             Progress(renderer, SPRITE_SIZE, HEIGHT - SPRITE_SIZE,
                      colors.white, Image(nullptr, ""), 0.0, 250.0f, 1.0)},
            {"fruit", fBar}};
    }
    void loadButtons()
    {
        vector<string> names = {"PLAY", "SETTINGS", "QUIT", "HOME"};
        map<string, std::function<void()>> functions = getBtnFuncs();
        for (int i = 0; i < names.size(); i++)
        {
            string label = names.at(i);
            buttons.emplace_back(
                renderer, WIDTH / 2, HEIGHT / 2 + (i * SPRITE_SIZE * 2),
                functions.at(label), label, colors.yellow);
        }
    }
    vector<string> getBtnNames()
    {
        vector<string> res = {};
        switch (game->state)
        {
        case States::HOME:
            res = {"PLAY", "SETTINGS", "QUIT"};
            break;
        }
        return res;
    }
    vector<string> getProgNames()
    {
        vector<string> res = {};
        switch (game->state)
        {
        case States::LOADING:
            res = {"loading"};
            break;
        case States::PLAYING:
            res = {"fruit"};
            break;
        }
        return res;
    }
    map<string, std::function<void()>> getBtnFuncs()
    {
        return {
            {"PLAY", [this]
             { game->update(States::PLAYING); }},
            {"SETTINGS", [this]
             { game->update(States::SETTINGS); }},
            {"QUIT", [this]
             { game->terminate(); }},
            {"HOME", [this]
             { game->update(States::HOME); }}};
    }
};