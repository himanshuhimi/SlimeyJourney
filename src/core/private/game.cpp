#include "../game.h"

Game::Game()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        print("SDL Unloaded: " + (string)SDL_GetError());
    if (!TTF_Init())
        print("TTF Unloaded: " + (string)SDL_GetError());
    if (!MIX_Init())
        print("Mix Unloaded: " + (string)SDL_GetError());
    float width = (scaled) ? CHANGED_WIDTH : WIDTH;
    float height = (scaled) ? CHANGED_HEIGHT : HEIGHT;
    if (!SDL_CreateWindowAndRenderer(TITLE.c_str(), width, height, 0, &window, &renderer))
        print("Display Unloaded: " + (string)SDL_GetError());
    if (scaled)
        SDL_SetRenderLogicalPresentation(renderer, WIDTH, HEIGHT, logicalPresentation);
    ui = new UI<Game *>(this);
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
            if (state == States::PLAYING)
                currentLevel->player.mouseClicked = (event.button.button == SDL_BUTTON_LEFT);
            break;
        }
        ui->update(event);
    }
    ui->handle(dt);
    switch (state)
    {
    case States::LOADING:
        if (ui->progresses.at("loading").complete)
        {
            state = nextState;
            ui->progresses.at("loading").reset();
        }
        break;
    case States::PLAYING:
        currentLevel->handle(dt);
        collision();
        break;
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(
        renderer, colors.skyblue.r,
        colors.skyblue.g, colors.skyblue.b,
        colors.skyblue.a);
    SDL_RenderClear(renderer);
    if (state == States::PLAYING)
        currentLevel->render();
    ui->render();
    SDL_RenderPresent(renderer);
}

void Game::update(States newState, bool loading)
{
    nextState = newState;
    state = (loading) ? States::LOADING : nextState;
    if (loading && nextState == States::PLAYING)
        loadLevels();
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

void Game::loadLevels()
{
    levels.clear();
    string directory = "data/maps";
    if (!fs::exists(directory) && !fs::is_directory(directory))
        return;
    for (const auto &entry : fs::directory_iterator(directory))
    {
        auto path = entry.path();
        if (path.filename().string() == "tilesets" || path.extension().string() != ".tmx")
            continue;
        string filename = path.stem().string();
        int number = std::stoi(filename);
        levels.emplace_back(new Level(renderer, number));
    }
    updateLevel();
}

void Game::updateLevel()
{
    levelNum++;
    if (levelNum <= levels.size())
        currentLevel = levels.at(levelNum - 1);
    else
    {
        update(States::HOME);
        levelNum = 0;
    }
}

void Game::collision()
{

    for (auto fruitIt = currentLevel->fruits.begin();
         fruitIt != currentLevel->fruits.end();)
        if (!fruitIt->picked && checkCollision(fruitIt->rect, currentLevel->player.rect))
        {
            currentLevel->player.audios.at("pickup").play();
            ui->progresses.at("fruit").update(currentLevel->increment);
            fruitIt = currentLevel->fruits.erase(fruitIt);
        }
        else
            fruitIt++;
    for (auto sIt = currentLevel->slimes.begin(); sIt != currentLevel->slimes.end();)
    {
        if (sIt->dead)
        {
            sIt = currentLevel->slimes.erase(sIt);
            continue;
        }
        if (checkCollision(currentLevel->player.rect, sIt->lineOfSight.rect))
        {
            sIt->actions.alert = true;
            if (checkCollision(currentLevel->player.rect, sIt->rect))
            {
                sIt->actions.attacking = true;
                currentLevel->player.inCombat = true;
                currentLevel->player.combatEnemy = &(*sIt);
            }
        }
        if (sIt->actions.attacking)
            sIt->attack(currentLevel->player.Center);
        for (auto bIt = sIt->balls.begin(); bIt != sIt->balls.end();)
            if (!bIt->used && checkCollision(currentLevel->player.rect, bIt->rect))
            {
                currentLevel->player.damage();
                bIt = sIt->balls.erase(bIt);
            }
            else
                bIt++;
        for (auto bIt = currentLevel->player.balls.begin();
             bIt != currentLevel->player.balls.end();)
            if (!bIt->used && checkCollision(sIt->rect, bIt->rect))
            {
                sIt->damage();
                bIt = currentLevel->player.balls.erase(bIt);
            }
            else
                bIt++;
        sIt++;
    }
    bool collided = checkCollision(currentLevel->player.rect, currentLevel->fren.rect);
    bool keyPressed = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_F];
    bool complete = ui->progresses.at("fruit").complete;
    currentLevel->quests.at("fruitColl").completed = complete;
    currentLevel->quests.at("fedFren").completed = collided & keyPressed;
    if (
        currentLevel->quests.at("fruitColl").completed &&
        currentLevel->quests.at("fedFren").completed)
        updateLevel();
}