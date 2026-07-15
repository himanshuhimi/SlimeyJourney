#include "../../systems/ui.h"

UIScreen::UIScreen(Game &game) : game(game) {}

void UIScreen::render(Vector2D Camera)
{
    for (auto &[category, widgets] : ctgWidgets)
        for (auto &[name, widget] : widgets)
        {
            if (!widget)
                continue;
            widget->render(Camera);
        }
}

void UIScreen::handle(double dt)
{
    for (auto &[category, widgets] : ctgWidgets)
        for (auto &[name, widget] : widgets)
        {
            if (!widget)
                continue;
            widget->handle(dt);
        }
}

void UIScreen::update(SDL_Event event)
{
    for (auto &[category, widgets] : ctgWidgets)
        for (auto &[name, widget] : widgets)
        {
            if (!widget)
                continue;
            widget->update(event);
        }
}

Hearts::Hearts(Game &game)
    : game(game), brokenHeart{game.renderer, "images/hearts/broken.png"},
      normalHeart(game.renderer, "images/hearts/normal.png")
{
    if (game.crntLvl == nullptr)
        return;
    load();
}

void Hearts::render()
{
    if (game.crntLvl == nullptr)
        return;
    for (int i = 0; i < images.size(); i++)
        images[i].render(nullptr, &rects[i]);
}

void Hearts::handle()
{
    if (game.crntLvl == nullptr)
        return;
    if (game.crntLvl->player.HP != game.crntLvl->player.maxHP)
        for (int i = 0; i < game.crntLvl->player.maxHP; i++)
            images[i] = (i < game.crntLvl->player.HP) ? normalHeart : brokenHeart;
}

void Hearts::clear()
{
    if (game.crntLvl == nullptr)
        return;
    images.clear();
    rects.clear();
}

void Hearts::load()
{
    float startX = (WIDTH / 2 - (game.crntLvl->player.maxHP / 2 * SPRITE_SIZE));
    if (images.empty())
        for (int i = 0; i < game.crntLvl->player.maxHP; i++)
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

HomeScreen::HomeScreen(Game &game) 
    : UIScreen(game), titleImage(game.renderer, "images/title.png")
{
    ctgWidgets["btns"];
    map<string, UIFunction> funcs = {
        {"PLAY", [this]
         { this->game.setScene(Scenes::PLAYING); }},
        {"SETTINGS", [this]
         { this->game.setScene(Scenes::SETTINGS); }},
        {"QUIT", [this]
         { this->game.terminate(); }}
        };
    int i = 0;
    for (auto &[label, func] : funcs)
    {
        float padding = i++ * SPRITE_SIZE * 2;
        ctgWidgets.at("btns").insert({label, make_unique<Button>(
            game.renderer, 
            WIDTH / 4 + padding, 
            HEIGHT / 2 + padding,
            func, 
            label, 
            colors.yellow
        )});
    }
    titleRect = SDL_FRect{(float)WIDTH, SPRITE_SIZE * 3, titleImage.width, titleImage.height};
    titleRect.x -= titleRect.w + SPRITE_SIZE;
}

void HomeScreen::render(Vector2D Camera)
{
    titleImage.render(nullptr, &titleRect);
    UIScreen::render(Camera);
}

LoadingScreen::LoadingScreen(Game &game) : UIScreen(game)
{
    ctgWidgets["progs"];
    ctgWidgets.at("progs").insert({
        "loading",
        make_unique<Progress>(
            game.renderer, SPRITE_SIZE, HEIGHT - SPRITE_SIZE,
            [this]{ this->game.setScene(this->game.nextScene, false); }
        )
    });
}

void LoadingScreen::handle(double dt)
{
    getWidget<Progress>("progs", "loading").advance(0.0025);
    UIScreen::handle(dt);
}

PlayingScreen::PlayingScreen(Game &game) : UIScreen(game), hearts(game)
{
    ctgWidgets["progs"];
    ctgWidgets.at("progs").insert({
        "fruit",
        make_unique<Progress>(
            game.renderer, WIDTH - SPRITE_SIZE, SPRITE_SIZE,
            [this]{ this->game.crntLvl->quests.at("fruit").completed = true; },
            colors.violet, Image(game.renderer, "ui/bottle.png")
        )
    });
    auto &fruit = getWidget<Progress>("progs", "fruit");
    fruit.rect.x -= fruit.rect.w + fruit.attachmentRect.w;
}

void PlayingScreen::render(Vector2D Camera)
{
    hearts.render();
    UIScreen::render(Camera);
}

void PlayingScreen::handle(double dt)
{
    hearts.handle();
    UIScreen::handle(dt);
}

UI::UI(Game &game) : game(game) {}

void UI::render(Vector2D Camera)
{
    if (activeScreen == nullptr)
        return;
    activeScreen->render(Camera);
}

void UI::handle(double dt)
{
    if (activeScreen == nullptr)
        return;
    activeScreen->handle(dt);
}

void UI::update(SDL_Event event)
{
    if (activeScreen == nullptr)
        return;
    activeScreen->update(event);
}

void UI::updateScreen(Scenes scene)
{
    switch (scene)
    {
    case Scenes::HOME:
        activeScreen = make_unique<HomeScreen>(game);
        break;
    case Scenes::LOADING:
        activeScreen = make_unique<LoadingScreen>(game);
        break;
    case Scenes::PLAYING:
        activeScreen = make_unique<PlayingScreen>(game);
        break;
    default:
        activeScreen = nullptr;
        break;
    };
}