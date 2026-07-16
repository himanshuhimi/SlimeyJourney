#include "../config.h"

const string TITLE = "Slimey Journey";
const float SPRITE_SIZE = 32.0f;
const int WIDTH{640}, HEIGHT{360};
int CHANGED_WIDTH{WIDTH}, CHANGED_HEIGHT{HEIGHT};
int scaleX{0}, scaleY{0};
int CAMERA_X{WIDTH / 2}, CAMERA_Y{HEIGHT / 2};
bool scaled = true;
const vector<string> fruitTypes = {
    "apple", "blueberry",
    "mango", "melon",
    "orange", "strawberry"};
SDL_RendererLogicalPresentation logicalPresentation = SDL_LOGICAL_PRESENTATION_INTEGER_SCALE;

void updateScale()
{
    scaleX = CHANGED_WIDTH / WIDTH;
    scaleY = CHANGED_HEIGHT / HEIGHT;
}

string capitalize(string s)
{
    if (!s.empty())
        s[0] = std::toupper(static_cast<unsigned char>(s[0]));
    return s;
}

bool checkCollision(SDL_FRect A, SDL_FRect B) { return SDL_HasRectIntersectionFloat(&A, &B); }

SDL_FRect getMousePosition()
{
    SDL_FRect result = {0, 0, 0, 0};
    SDL_GetMouseState(&result.x, &result.y);
    if (scaled)
        result.x /= scaleX;
    result.y /= scaleY;
    return result;
}

vector<string> partition(string splittingStr, string separator)
{
    vector<string> result = {};
    size_t pos = splittingStr.find(separator);
    if (pos != string::npos)
    {
        string left = splittingStr.substr(0, pos);
        string middle = separator;
        string right = splittingStr.substr(pos + separator.size());
        result.emplace_back(left);
        result.emplace_back(middle);
        result.emplace_back(right);
    }
    return result;
}