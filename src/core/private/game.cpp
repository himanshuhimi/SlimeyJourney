#include "../game.h"

Game::Game()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        log("SDL Unloaded: " + (string)SDL_GetError());
    else
        log("SDL Loaded");
    if (!TTF_Init())
        log("TTF Unloaded: " + (string)SDL_GetError());
    else
        log("TTF Loaded");
    if (!MIX_Init())
        log("Mix Unloaded: " + (string)SDL_GetError());
    else
        log("Mix Loaded");
    if (!SDL_CreateWindowAndRenderer(TITLE.c_str(), WIDTH, HEIGHT, 0, &window, &renderer))
        log("Display Unloaded: " + (string)SDL_GetError());
    else
        log("Display Loaded");
    level = new Level(renderer, 1);
    audios = {
        {"pickup", new Audio("assets/audios/pickup.wav")}};
    active = true;
}

void Game::launch()
{
    while (active)
    {
        handle();
        render();
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(
        renderer,
        colors.skyblue.r,
        colors.skyblue.g,
        colors.skyblue.b,
        colors.skyblue.a
    );
    SDL_RenderClear(renderer);
    level->render();
    SDL_RenderPresent(renderer);
}

void Game::handle()
{
    updateDeltaTime();
    while (SDL_PollEvent(&event))
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            terminate();
    level->handle(dt);
    for (
        auto fruitIt = level->fruits.begin();
        fruitIt != level->fruits.end();)
    {
        bool collided = checkCollision(fruitIt->rect, level->player.rect);
        if (!fruitIt->taken && collided)
        {
            fruitIt->taken = true;
            level->points += 1;
            audios["pickup"]->play();
        }
        else
            fruitIt++;
    }
}

void Game::terminate()
{
    active = false;
    SDL_Quit();
}

void Game::updateDeltaTime()
{
    NOW = SDL_GetPerformanceCounter();
    dt = (double)(NOW - LAST) / SDL_GetPerformanceFrequency();
    LAST = NOW;
}