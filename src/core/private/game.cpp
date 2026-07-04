#include "../game.h"

Game::Game()
{
    settings = new Settings();
    string size = settings->get("graphics", "size");
    vector<string> partitioned = partition(size, "x");
    CHANGED_WIDTH = std::stoi(partitioned.at(0));
    CHANGED_HEIGHT = std::stoi(partitioned.at(partitioned.size() - 1));
    updateScale();
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
    SDL_SetRenderVSync(renderer, std::stoi(settings->get("graphics", "vsync")));
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
        ui->hearts.clear();
    }
}

void Game::collision()
{

    for (auto fruitIt = currentLevel->fruits.begin();
         fruitIt != currentLevel->fruits.end();)
    {
        auto &fruit = *fruitIt;
        if (!fruit->picked && checkCollision(fruit->rect, currentLevel->player.rect))
        {
            currentLevel->player.audios.at("pickup").play();
            ui->progresses.at("fruit").advance(currentLevel->increment);
            fruitIt = currentLevel->fruits.erase(fruitIt);
        }
        else
            fruitIt++;
    }
    for (auto sIt = currentLevel->enemies.begin(); sIt != currentLevel->enemies.end();)
    {
        auto &slime = *sIt;
        auto rawSlime = slime.get();
        if (slime->dead)
        {
            slime->drop<Fruit>(currentLevel->fruits);
            sIt = currentLevel->enemies.erase(sIt);
            continue;
        }
        if (checkCollision(currentLevel->player.rect, slime->range))
        {
            slime->actions.alert = true;
            slime->actions.attacking = true;
            currentLevel->player.inCombat = true;
            currentLevel->player.combatEnemy = rawSlime;
        }
        else if (currentLevel->player.inCombat && currentLevel->player.combatEnemy == rawSlime)
        {
            currentLevel->player.inCombat = false;
            currentLevel->player.combatEnemy = nullptr;
            slime->actions.attacking = false;
            slime->actions.alert = false;
        }
        if (slime->actions.attacking)
            slime->attack((currentLevel->player.Position - slime->Position).normalise());
        for (auto bIt = slime->balls.begin(); bIt != slime->balls.end();)
            if (!bIt->used && checkCollision(currentLevel->player.rect, bIt->rect))
            {
                currentLevel->player.damage();
                bIt = slime->balls.erase(bIt);
            }
            else
                bIt++;
        for (auto bIt = currentLevel->player.balls.begin();
             bIt != currentLevel->player.balls.end();)
            if (!bIt->used && checkCollision(slime->rect, bIt->rect))
            {
                slime->damage();
                bIt = currentLevel->player.balls.erase(bIt);
            }
            else
                bIt++;
        sIt++;
    }
    bool collided = checkCollision(currentLevel->player.rect, currentLevel->fren.rect);
    bool keyPressed = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_F];
    bool complete = ui->progresses.at("fruit").complete;
    if (complete)
        currentLevel->quests.at("fruitColl").completed = true;
    if (collided && keyPressed && currentLevel->quests.at("fruitColl").completed)
        currentLevel->quests.at("fedFren").completed = true;
    bool allEnemyDead = std::all_of(currentLevel->enemies.begin(),
                                    currentLevel->enemies.end(),
                                    [](const auto &e)
                                    { return e->dead; });
    if (allEnemyDead)
        currentLevel->quests.at("killEnemy").completed = true;
    bool allComplete = std::all_of(currentLevel->quests.begin(),
                                   currentLevel->quests.end(), [](const auto &entry)
                                   { return entry.second.completed; });
    if (allComplete)
        updateLevel();
    if (currentLevel->player.dead)
        update(States::HOME);
}