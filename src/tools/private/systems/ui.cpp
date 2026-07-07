#include "../../systems/ui.h"

Hearts::Hearts(Game &game) 
    : game(game), brokenHeart{game.renderer, "images/hearts/broken.png"},
      normalHeart(game.renderer, "images/hearts/normal.png")
{
    if (game.currentLevel == nullptr)
        return;
    load();
}

void Hearts::render()
{
    if (game.currentLevel == nullptr)
        return;
    for (int i = 0; i < images.size(); i++)
        images[i].render(nullptr, &rects[i]);
}

void Hearts::handle()
{
    if (game.currentLevel == nullptr)
        return;
    if (game.currentLevel->player.HP != game.currentLevel->player.maxHP)
        for (int i = 0; i < game.currentLevel->player.maxHP; i++)
            images[i] = (i < game.currentLevel->player.HP) ? normalHeart : brokenHeart;
}

void Hearts::clear()
{
    if (game.currentLevel == nullptr)
        return;
    images.clear();
    rects.clear();
}

void Hearts::load()
{
    float startX = (WIDTH / 2 - (game.currentLevel->player.maxHP / 2 * SPRITE_SIZE));
    if (images.empty())
        for (int i = 0; i < game.currentLevel->player.maxHP; i++)
        {
            images.emplace_back(normalHeart);
            rects.emplace_back(
                SDL_FRect{
                    startX + (i * SPRITE_SIZE),
                    HEIGHT - SPRITE_SIZE,
                    images[i].width,
                    images[i].height});
        };
}

UI::UI(Game &game) : game(game), renderer(game.renderer), hearts(game)
{
    loadProgresses();
    loadButtons();
    loadTexts();
}

void UI::handle(double dt)
{
    switch (game.state)
    {
    case States::LOADING:
        progresses.at("loading").handle(dt);
        break;
    case States::PLAYING:
        hearts.handle();
        break;
    }
    vector<string> curProgs = getProgNames(game.state);
    for (auto &prog : curProgs)
        progresses.at(prog).handle(dt);
    vector<string> curBtns = getBtnNames(game.state);
    for (auto &button : curBtns)
        buttons.at(button).handle(dt);
}

void UI::render()
{
    switch (game.state)
    {
    case States::LOADING:
        progresses.at("loading").render();
        break;
    case States::PLAYING:
        hearts.render();
        break;
    }
    vector<string> curProgs = getProgNames(game.state);
    for (auto &name : curProgs)
        progresses.at(name).render();
    vector<string> curBtns = getBtnNames(game.state);
    for (auto &button : curBtns)
        buttons.at(button).render();
    for (auto &text : texts)
    {
        vector<string> curText = getTitleNames();
        if (std::find(curText.begin(), curText.end(), text.data) != curText.end())
            text.render();
    }
    if (game.state == States::LOADING)
        progresses.at("loading").advance();
}

void UI::update(SDL_Event event)
{
    vector<string> curBtns = getBtnNames(game.state);
    for (auto &button : curBtns)
        buttons.at(button).update(event);
}

void UI::loadProgresses()
{
    progresses.clear();
    const auto &progFuncs = getProgFuncs();
    Progress fBar = Progress(renderer, WIDTH - SPRITE_SIZE, HEIGHT / 16.0f,
                             getByName(progFuncs, "fruit"),
                             SDL_Color{95, 90, 204, 255},
                             Image(renderer, "ui/bottle.png"));
    fBar.rect.x -= fBar.rect.w + fBar.attachmentRect.w;
    progresses = {
        {"loading",
         Progress(renderer, SPRITE_SIZE, HEIGHT - SPRITE_SIZE, 
            getByName(progFuncs, "loading"), colors.white, Image(nullptr, ""), 
            0.0, 250.0f, 1.0)},
        {"fruit", fBar}};
}

void UI::loadButtons()
{
    const auto &functions = getBtnFuncs();
    int i = 0;
    for (const auto &[label, function] : functions)
        buttons.insert({label,
                        Button(renderer,
                               WIDTH / 2,
                               HEIGHT / 2 + (i++ * 2 * SPRITE_SIZE),
                               function, label, colors.yellow)});
}

void UI::loadTexts()
{
    vector<string> data = {
        TITLE,
        "COMPLETED",
        "GAME OVER!"};
    for (auto &str : data)
        texts.emplace_back(renderer, WIDTH / 2, 96, str, colors.white, 48);
}

vector<string> UI::getBtnNames(States state)
{
    map<States, vector<string>> data = {
        {States::HOME, {"PLAY", "SETTINGS", "QUIT"}},
        {States::OVER, {"TRY AGAIN", "HOME"}}};
    auto iterator = data.find(state);
    if (iterator == data.end())
        return {};
    return iterator->second;
}

vector<string> UI::getProgNames(States state)
{
    map<States, vector<string>> data = {
        {States::LOADING, {"loading"}},
        {States::PLAYING, {"fruit"}}};
    auto iterator = data.find(state);
    if (iterator == data.end())
        return {};
    return iterator->second;
}

vector<string> UI::getTitleNames()
{
    vector<string> res = {};
    switch (game.state)
    {
    case States::HOME:
        res = {(string)TITLE};
        break;
    }
    return res;
}

vector<pair<string, UIFunction>> UI::getBtnFuncs()
{
    return {
        {"PLAY", [this]
         { game.update(States::PLAYING); }},
        {"SETTINGS", [this]
         { game.update(States::SETTINGS); }},
        {"QUIT", [this]
         { game.terminate(); }},
        {"TRY AGAIN", [this]
         { game.update(States::PLAYING); }},
        {"PLAY AGAIN", [this]
         { game.update(States::PLAYING); }},
        {"HOME", [this]
         { game.update(States::HOME); }}};
}

vector<pair<string, UIFunction>> UI::getProgFuncs()
{
    return {
        {"loading", [this]()
         { this->game.update(this->game.nextState, false); }},
        {"fruit",
         [this]()
         { this->game.currentLevel->quests.at("fruitColl").completed = true; }}};
}