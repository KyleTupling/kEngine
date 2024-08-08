#include "Text.h"

Text::Text(SDL_Renderer* renderer, std::string message, TTF_Font* textFont, int posX, int posY)
{
    this->renderer = renderer;
    this->message = &message[0];
    this->font = textFont;

    this->posX = posX;
    this->posY = posY;

    SDL_Surface* surface = TTF_RenderText_Blended(this->font, &this->message[0], this->color);
    this->texture = SDL_CreateTextureFromSurface(this->renderer, surface);
    SDL_FreeSurface(surface);

    this->textureWidth = 0;
    this->textureHeight = 0;
    SDL_QueryTexture(this->texture, NULL, NULL, &this->textureWidth, &this->textureHeight);
    this->boundingRect = { static_cast<int>(this->posX - this->textureWidth / 2), static_cast<int>(this->posY - this->textureHeight / 2), this->textureWidth, this->textureHeight };
}

Text::Text(SDL_Renderer* renderer, std::string message, TTF_Font* textFont, int posX, int posY, SDL_Color color)
{
    this->renderer = renderer;
    this->message = &message[0];
    this->font = textFont;

    this->posX = posX;
    this->posY = posY;

    this->color = color;

    SDL_Surface* surface = TTF_RenderText_Blended(this->font, &this->message[0], this->color);
    this->texture = SDL_CreateTextureFromSurface(this->renderer, surface);
    SDL_FreeSurface(surface);

    this->textureWidth = 0;
    this->textureHeight = 0;
    SDL_QueryTexture(this->texture, NULL, NULL, &this->textureWidth, &this->textureHeight);
    this->boundingRect = { static_cast<int>(this->posX - this->textureWidth / 2), static_cast<int>(this->posY - this->textureHeight / 2), this->textureWidth, this->textureHeight };
}

Text::Text(const Text& other)
{
    updateTexture();
}

Text& Text::operator=(const Text& other)
{
    if (this == &other) return *this; // Check if instance is same as other

    this->renderer = other.renderer;
    this->message = other.message;
    this->font = other.font;
    this->posX = other.posX;
    this->posY = other.posY;
    this->color = other.color;

    this->updateTexture();

    return *this;
}

Text::~Text()
{
    if (this->texture) SDL_DestroyTexture(this->texture);
}

void Text::Update()
{
    this->boundingRect = { static_cast<int>(this->posX - this->textureWidth / 2), static_cast<int>(this->posY - this->textureHeight / 2), this->textureWidth, this->textureHeight };
}

void Text::Draw(const Camera& camera, const Vector2D& screenSize)
{
    SDL_Rect renderBound;
    // Create new bounding rect to account for screen position
    if (!isFixed)
    {
        Vector2D screenPos = camera.ConvertWorldToScreen(Vector2D(posX, posY), screenSize);
        renderBound = { static_cast<int>(screenPos.x - this->textureWidth / 2), static_cast<int>(screenPos.y - this->textureHeight / 2), this->textureWidth, this->textureHeight };
    }
    else 
    {
        renderBound = boundingRect;
    }
    
    SDL_RenderCopy(this->renderer, this->texture, NULL, &renderBound);
}

void Text::updateTexture()
{
    if (this->texture)
    {
        SDL_DestroyTexture(this->texture);
    }

    SDL_Surface* surface = TTF_RenderText_Solid(this->font, &this->message[0], this->color);
    this->texture = SDL_CreateTextureFromSurface(this->renderer, surface);
    SDL_FreeSurface(surface);

    SDL_QueryTexture(this->texture, NULL, NULL, &this->textureWidth, &this->textureHeight);
}

// Setters

void Text::setMessage(char* message)
{
    this->message = message;
    this->updateTexture();
}

void Text::setMessage(std::string message)
{
    this->message = &message[0];
    this->updateTexture();
}

void Text::setFont(TTF_Font* textFont)
{
    this->font = textFont;
}

void Text::setTexture(SDL_Texture* texture)
{
    this->texture = texture;
}

void Text::setPosX(int posX)
{
    this->posX = posX;
}

void Text::setPosY(int posY)
{
    this->posY = posY;
}

void Text::setIsFixed(bool fixed)
{
    isFixed = fixed;
}

// Getters

char* Text::getMessage()
{
    return this->message;
}

TTF_Font* Text::getFont()
{
    return this->font;
}

SDL_Texture* Text::getTexture()
{
    return this->texture;
}

int Text::getPosX()
{
    return this->posX;
}

int Text::getPosY()
{
    return this->posY;
}

bool Text::getIsFixed()
{
    return this->isFixed;
}