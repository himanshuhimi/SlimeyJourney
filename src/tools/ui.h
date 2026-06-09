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
        vector<string> labels = getBtnLabels();
        for (int i = 0; i < labels.size(); i++)
        {
            string label = labels[i];
            std::function<void()> callback = getBtnFunction(label);
            float x = WIDTH / 2;
            float y = HEIGHT / 2 + (i * 64);
            SDL_Color color = getBtnColor(label);
            buttons.emplace_back(game->renderer, x, y, callback, label, color);
        }
    }

    vector<string> getBtnLabels()
    {
        vector<string> res = {};
        switch (game->state)
        {
        case States::HOME:
            res = {"PLAY", "SETTINGS", "QUIT"};
            break;
        case States::SETTINGS:
            break;
        case States::PAUSED:
            res = {"CONTINUE", "HOME", "QUIT"};
            break;
        case States::PROGRESSING:
            res = {"NEXT", "PLAY AGAIN", "HOME"};
            break;
        case States::OVER:
            res = {"TRY AGAIN", "HOME", "QUIT"};
            break;
        };
        return res;
    }

    std::function<void()> getBtnFunction(string label)
    {
        map<string, std::function<void()>> functions =
            {
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
        return functions.at(label);
    }

    SDL_Color getBtnColor(string label)
    {
        SDL_Color normal = SDL_Color{211, 211, 255, 255};
        map<string, SDL_Color> cols = {
            {"PLAY", colors.green},
            {"NEXT", colors.green},
            {"TRY AGAIN", colors.green},
            {"PLAY AGAIN", colors.green},
            {"CONTINUE", colors.green},
            {"QUIT", colors.red},
            {"SETTINGS", normal},
            {"HOME", normal},
        };
        return cols.at(label);
    }
};