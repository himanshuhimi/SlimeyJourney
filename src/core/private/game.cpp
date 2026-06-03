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
        colors.skyblue.a);
    SDL_RenderClear(renderer);
    level->render();
    SDL_RenderPresent(renderer);
}

void Game::handle()
{
    updateDeltaTime();
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            terminate();
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            level->player.mouseClicked = (event.button.button == SDL_BUTTON_LEFT);
        }
    }
    collision();
    level->handle(dt);
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

void Game::collision()
{
    for (auto fruitIt = level->fruits.begin(); fruitIt != level->fruits.end();)
        if (!fruitIt->picked && checkCollision(fruitIt->rect, level->player.rect))
        {
            fruitIt->picked = true;
            level->points += 1;
            audios["pickup"]->play();
            level->fruitProg.update(level->increment);
        }
        else
            fruitIt++;
    for (auto ballIt = level->player.balls.begin(); ballIt != level->player.balls.end(); ballIt++)
        for (auto enemyIt = level->enemies.begin(); enemyIt != level->enemies.end();)
        {
            if (!ballIt->used && checkCollision(ballIt->rect, enemyIt->rect))
            {
                enemyIt->healthPoints -= 1;
                enemyIt->healthBar.update(-(double)1 / enemyIt->maxHealthPoints);
                ballIt->used = true;
            }
            else
                enemyIt++;
        }
    for (auto enemyIt = level->enemies.begin(); enemyIt != level->enemies.end(); enemyIt++)
        enemyIt->drop<Fruit>(level->fruits);
}