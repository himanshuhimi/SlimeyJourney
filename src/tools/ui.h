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
    Level *currentLevel = nullptr;
    vector<Image *> hearts;
    Image *normalHeart, *brokenHeart;
    map<string, Progress> progresses = {};
    vector<Button> buttons = {};
    T game;
    UI(T game) : game(game), renderer(game->renderer)
    {
        currentLevel = game->currentLevel;
        normalHeart = new Image{renderer, "images/hearts/normal.png"};
        brokenHeart = new Image{renderer, "images/hearts/broken.png"};
        Progress fBar = Progress(renderer, WIDTH - SPRITE_SIZE, HEIGHT / 16.0f,
                                 SDL_Color{95, 90, 204, 255}, Image(renderer, "ui/bottle.png"));
        fBar.rect.x -= fBar.rect.w + fBar.attachmentRect.w;
        progresses = {{"fruit", fBar}};
        vector names = {"PLAY", "SETTINGS", "QUIT", "HOME"};
        // for (int i = 0; i < names.size(); i++)
    }
    void handle(double dt)
    {
        switch (game->state)
        {
        case States::PLAYING:
        {
            if (hearts.size() < currentLevel->player.maxHP)
                for (int i = 0; i < currentLevel->player.maxHP; i++)
                    hearts.emplace_back(normalHeart);
            if (currentLevel->player.HP != currentLevel->player.maxHP)
                hearts.at(currentLevel->player.HP) = brokenHeart;
            for (auto &[_, progress] : progresses)
                progress.handle(dt);
        }
        }
    }
    void render()
    {
        for (int i = 0; i < currentLevel->player.maxHP; i++)
        {
            float startX = (WIDTH / 2 - (currentLevel->player.maxHP / 2 * SPRITE_SIZE));
            hearts.at(i)->render(
                nullptr,
                new SDL_FRect{
                    startX + (i * SPRITE_SIZE),
                    HEIGHT - SPRITE_SIZE,
                    hearts.at(i)->width,
                    hearts.at(i)->height});
        }
        for (auto &[_, progress] : progresses)
            progress.render();
    }
    vector<string> getButtonNames()
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
};