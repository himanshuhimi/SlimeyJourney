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
    ui = new UI(*this);
    active = true;
    SDL_AddTimer(5000, passiveTimerCallback, this);
    audios = {
        {"success", Audio("success.wav")}
    };
    setScene(Scenes::HOME, false);
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
            if (scene == Scenes::PLAYING)
                crntLvl->player.mouseClicked = (event.button.button == SDL_BUTTON_LEFT);
            break;
        case SDL_EVENT_KEY_UP:
            if (event.key.key == SDLK_ESCAPE)
            {
                if (scene != Scenes::PLAYING &&
                scene != Scenes::LOADING &&
                scene != Scenes::HOME) 
                setScene(Scenes::HOME);
                if (scene == Scenes::PLAYING)
                    setScene(Scenes::PAUSED, false);
            }
            break;
        case SDL_EVENT_USER:
            if (event.type == PASSIVE_EVENT)
            {
                if (clouds.size() > 99 || petals.size() > 99)
                {
                    clouds.clear();
                    petals.clear();
                }
                if (Random.randint(0, 1))
                    clouds.emplace_back(Cloud(renderer));
                else
                    petals.emplace_back(Petal(renderer));
            }
            break;
        }
        ui->update(event);
    }
    for (auto &cloud : clouds)
        cloud.handle(dt);
    for (auto &petal : petals)
        petal.handle(dt);
    ui->handle(dt);
    switch (scene)
    {
    case Scenes::PLAYING:
        crntLvl->handle(dt);
        collision();
        break;
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
    for (auto &cloud : clouds)
        cloud.render((scene == Scenes::PLAYING) ? crntLvl->Camera : Vector2D{});
    for (auto &petal : petals)
        petal.render((scene == Scenes::PLAYING) ? crntLvl->Camera : Vector2D{});
    if (scene == Scenes::PLAYING)
        crntLvl->render();
    ui->render();
    SDL_RenderPresent(renderer);
}

void Game::setScene(Scenes newScene, bool loading)
{
    prevScene = scene;
    nextScene = newScene;
    scene = (loading) ? Scenes::LOADING : nextScene;
    if (loading)
    {
        if (nextScene == Scenes::SELECTION)
            loadLevels();
        else if (nextScene != Scenes::PLAYING)
            unloadLevels();
    }
    ui->updateScreen(scene);
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
    unloadLevels();
    string directory = "data/maps";
    if (!fs::exists(directory))
        return;
    for (const auto &rgnEntry : fs::directory_iterator(directory))
    {
        auto rgnPath = rgnEntry.path();
        string rgnFilename = rgnPath.filename().string();
        if (rgnFilename == "tilesets")
            continue;
        string region = rgnPath.stem().string();
        if (fs::is_directory(rgnPath))
        {
            regions[region];
            lvlNums[region];
            for (auto &tmx : fs::directory_iterator(rgnPath.string()))
            {
                auto tmxPath = tmx.path();
                string tmxFilename = tmxPath.filename().string();
                int number = std::stoi(tmxPath.stem().string());
                regions.at(region).emplace(number, new Level(renderer, region, number));
                lvlNums.at(region).emplace_back(number);
            }
        }
    }
    for (auto &[region, nums] : lvlNums)
        rgnMaxLvls[region] = *std::max_element(nums.begin(), nums.end());
}

void Game::unloadLevels()
{
    regions.clear();
}

void Game::setLevel(string region, int number)
{
    crntRgnName = region;
    lvlNum = number;
    auto &crntLvls = regions.at(crntRgnName);
    auto it = crntLvls.find(lvlNum);
    if (it != crntLvls.end())
        crntLvl = it->second;
    else
        setScene(Scenes::SELECTION);
}

void Game::nextLevel()
{
    lvlNum++;
    setLevel(crntRgnName, lvlNum);
    ui->updateScreen(scene);
}

void Game::collision()
{
    auto &level = crntLvl;
    if (crntLvl->stones.size() > 0)
        for (auto stoneIt = level->stones.begin();
            stoneIt != level->stones.end(); stoneIt++)
        {
            auto stone = *stoneIt;
            for (auto bIt = level->player.balls.begin();
                bIt != level->player.balls.end();)
            {
                auto ball = *bIt;
                if (checkCollision(stone.rect, ball.rect))
                {
                    stoneIt->destruct();
                    bIt = level->player.balls.erase(bIt);
                }
                else
                    bIt++;
            }
        }
    if (crntLvl->spikes.size() > 0)
        for (auto &spike : level->spikes)
            if (checkCollision(level->player.rect, spike.rect))
                level->player.resetPos();
    if (crntLvl->pads.size() > 0)
    {
        bool onPad = false;
        for (auto &pad : level->pads)
            if (checkCollision(level->player.rect, pad.rect) && !level->player.buffed)
            {
                onPad = true;
                break;
            }
        if (onPad && !level->player.buffed)
        {
            level->player.jumpStrength += 0.2 * level->player.baseJumpStrength;
            level->player.buffed = true;
        }
    }
    if (crntLvl->fruits.size() > 0)
        for (auto fruitIt = level->fruits.begin();
            fruitIt != level->fruits.end();)
        {
            auto &fruit = *fruitIt;
            if (!fruit->picked && checkCollision(fruit->rect, level->player.rect))
            {
                level->player.audios.at("pickup").play();
                ui->getWidget<Progress>("progs", "fruit").advance(level->increment);
                fruitIt = level->fruits.erase(fruitIt);
            }
            else
                fruitIt++;
        }
    if (crntLvl->enemies.size() > 0)
        for (auto eIt = level->enemies.begin(); eIt != level->enemies.end();)
        {
            auto &enemy = *eIt;
            auto rawSlime = enemy.get();
            if (enemy->dead)
            {
                enemy->drop<Fruit>(level->fruits);
                eIt = level->enemies.erase(eIt);
                level->player.enemiesKilled++;
                if (level->player.HP != level->player.maxHP)
                    level->player.HP++;
                continue;
            }
            if (checkCollision(level->player.rect, enemy->range))
            {
                enemy->actions.alert = true;
                enemy->actions.attacking = true;
                level->player.inCombat = true;
                level->player.combatEnemy = rawSlime;
            }
            else if (level->player.inCombat && level->player.combatEnemy == rawSlime)
            {
                level->player.inCombat = false;
                level->player.combatEnemy = nullptr;
                enemy->actions.attacking = false;
                enemy->actions.alert = false;
            }
            if (enemy->actions.attacking)
                enemy->attack((level->player.Position - enemy->Position).normalise());
            for (auto wIt = enemy->throws.begin(); wIt != enemy->throws.end();)
            {
                auto &weapon = *wIt->get();
                if (!weapon.used && checkCollision(level->player.rect, weapon.rect))
                {
                    level->player.damage();
                    wIt = enemy->throws.erase(wIt);
                }
                else
                    wIt++;
            }
            for (auto bIt = level->player.balls.begin();
                bIt != level->player.balls.end();)
                if (!bIt->used && checkCollision(enemy->rect, bIt->rect))
                {
                    enemy->damage();
                    bIt = level->player.balls.erase(bIt);
                }
                else
                    bIt++;
            eIt++;
        }
    bool keyPressed = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_F];
    bool collided = checkCollision(level->player.rect, level->fren.rect);
    if (keyPressed && collided)
    {
        auto frenQuest = &level->quests.at("fed");
        frenQuest->completed = true;
        if (level->fruitLength > 0 && ui->getWidget<Progress>("progs", "fruit").complete)
            level->quests.at("fruit").completed = true;
        if (level->enemyLength > 0)
            if (level->enemyLength >= level->player.enemiesKilled)
                level->quests.at("enemies").completed = true;
        bool complete = std::all_of(level->quests.begin(),
                                    level->quests.end(),
                                    [](auto &q)
                                    { return q.second.completed; });
        if (complete)
        {
            nextLevel();
            audios.at("success").play();
        }
    }
    if (level->player.dead || crntLvl->timer.currentTime <= 0.0)
        setScene(Scenes::OVER, false);
}

Uint32 Game::passiveTimerCallback(void *userdata, SDL_TimerID id, Uint32 interval)
{
    Game *game = static_cast<Game *>(userdata);
    SDL_Event passiveEvent;
    SDL_zero(passiveEvent);
    passiveEvent.type = game->PASSIVE_EVENT;
    SDL_PushEvent(&passiveEvent);
    return Random.randint(0, 5000);
};