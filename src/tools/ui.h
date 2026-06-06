#pragma once

#include "core/config.h"
#include "ui/button.h"

template <typename T>
class UI
{
public:
    Progress loading;
    vector<Button> buttons = {};
    T game;

    UI(T game)
        : game(game), loading(game->renderer, WIDTH / 2, HEIGHT - 50,
                              colors.white, Image{nullptr, ""}, 0.0, WIDTH - SPRITE_SIZE)
    {
        loadButtons();
    }

    void handle(SDL_Event event)
    {
        for (auto &button : buttons)
            button.handle(event);
    }

    void render()
    {
        for (auto &button : buttons)
            button.render();
    }

    void update(double dt)
    {
        for (auto &button : buttons)
            button.update(dt);
    }

    void loadButtons()
    {
        if (!buttons.empty())
            buttons.clear();
        vector<string> labels = getButtonLabels();
        map<string, std::function<void()>> functions = getButtonFunctions();
        for (int i = 0; i < labels.size(); i++)
            buttons.emplace_back(game->renderer, WIDTH / 2, HEIGHT / 2 + (i * 60),
                                 functions[labels[i]], labels[i]);
    }

    vector<string> getButtonLabels()
    {
        vector<string> result = {};
        switch (game->state)
        {
        case States::HOME:
            result = {"PLAY", "SETTINGS", "QUIT"};
            break;
        case States::SETTINGS:
            break;
        case States::PAUSED:
            result = {"CONTINUE", "HOME", "QUIT"};
            break;
        case States::PROGRESSING:
            result = {"NEXT", "PLAY AGAIN", "HOME"};
            break;
        case States::OVER:
            result = {"TRY AGAIN", "HOME", "QUIT"};
            break;
        }
        return result;
    }

    map<string, std::function<void()>> getButtonFunctions()
    {
        return {
            {"PLAY", [this]
             { game->update(States::PLAYING); }},
            {"TRY AGAIN", [this]
             { game->update(States::PLAYING); }},
            {"PLAY AGAIN", [this]
             { game->update(States::PLAYING); }},
            {"CONTINUE", [this]
             { game->update(States::PLAYING, false); }},
            {"SETTINGS", [this]
             { game->update(States::SETTINGS); }},
            {"HOME", [this]
             { game->update(States::HOME); }},
            {"NEXT", [this]
             { game->nextLevel(); }},
            {"QUIT", [this]
             { game->terminate(); }},
        };
    }
};