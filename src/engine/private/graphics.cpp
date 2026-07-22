#include "../graphics.h"

Image::Image(SDL_Renderer *renderer, string source) : renderer(renderer), source(source)
{
    string path = "data/assets/" + source;
    if (!renderer)
        return;
    surface = IMG_Load(path.c_str());
    if (!surface)
        std::cout << "Unloaded Image: " + (string)SDL_GetError();
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_GetTextureSize(texture, &width, &height);
}

void Image::render(const SDL_FRect *src, const SDL_FRect *dst)
{
    SDL_RenderTexture(renderer, texture, src, dst);
}

Animation::Animation(SDL_Renderer *renderer, string source, float frameTime)
    : renderer(renderer), frameTime(frameTime), imageSet(renderer, "anims/" + source),
      maxFrames(imageSet.width / 32) {}

void Animation::handle(double dt)
{
    if (!active)
        return;
    timer += (float)dt;
    if (timer >= frameTime)
    {
        timer = 0.0f;
        index++;
        if (index >= maxFrames)
        {
            index = 0;
            complete = true;
            active = false;
        }
    };
}

void Animation::render(Vector2D Camera, SDL_FRect dst)
{
    SDL_FRect renderDst = dst;
    renderDst.x -= Camera.x;
    renderDst.y -= Camera.y;
    src.x = index * dst.w;
    src.y = 0;
    src.w = (float)dst.w;
    src.h = (float)dst.h;
    imageSet.render(&src, &renderDst);
}

void Animation::restart()
{
    index = 0;
    timer = 0;
    complete = false;
    active = true;
}

Text::Text(
    SDL_Renderer *renderer,
    float x, float y, string data, SDL_Color color,
    int pixelSize, int posMode, Image attachment, string fontSource)
    : renderer(renderer), x(x), y(y), data(data), pixelSize(pixelSize), color(color),
      attachment(attachment)
{
    string path = "data/assets/" + fontSource;
    font = TTF_OpenFont(path.c_str(), pixelSize);
    if (!font)
        std::cout << "Font Uninitialized: " + path;
    Text::posMode = static_cast<PositionModes>(posMode);
    surface = TTF_RenderText_Blended(font, data.c_str(), data.size(), color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    SDL_GetTextureSize(texture, &rect.w, &rect.h);
    if (posMode == PositionModes::CENTERED)
    {
        rect.x = x - (rect.w / 2);
        rect.y = y - (rect.h / 2);
    }
    else if (posMode == PositionModes::TOPLEFT)
    {
        rect.x = x;
        rect.y = y;
    }
    if (attachment.renderer != nullptr)
    {
        attachRect.w = attachment.width;
        attachRect.h = attachment.height;
        attachRect.x = rect.x - (attachRect.w + attachRect.w / 2);
        attachRect.y = rect.y;
    }
}

void Text::render(Vector2D Camera)
{
    dst = rect;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    if (attachment.renderer != nullptr)
    {
        SDL_FRect attachDst = attachRect;
        attachDst.x -= Camera.x;
        attachDst.y -= Camera.y;
        attachment.render(nullptr, &attachDst);
    }
    SDL_RenderTexture(renderer, texture, nullptr, &dst);
}

void Text::updateData(string newData)
{
    if (texture)
        SDL_DestroyTexture(texture);
    surface = TTF_RenderText_Blended(font, newData.c_str(), newData.size(), color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_GetTextureSize(texture, &rect.w, &rect.h);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
}

void Text::updateAlpha(int newAlpha)
{
    alpha = newAlpha;
    if (texture)
        SDL_SetTextureAlphaMod(texture, alpha);
}

void Text::updateAttach(Image newAttach) { attachment = newAttach; }

Object::Object(float x, float y, float width, float height)
{
    Position.x = x;
    Position.y = y;
    rect.x = Position.x;
    rect.y = Position.y;
    rect.w = width;
    rect.h = height;
}