#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Camera.h"

class Text
{
private:
    char* message;
    TTF_Font* font;
    SDL_Texture* texture;
    int textureWidth;
    int textureHeight;

    SDL_Color color = { 0, 0, 0, 255 };

    SDL_Rect boundingRect;

    SDL_Renderer* renderer;

    int posX;
    int posY;

    bool isFixed = false;

    void updateTexture();

public:
    Text(SDL_Renderer* renderer, std::string message, TTF_Font* textFont, int posX, int posY);
    Text(SDL_Renderer* renderer, std::string message, TTF_Font* textFont, int posX, int posY, SDL_Color color);
    Text(const Text& other); // Copy constructor
    Text& operator=(const Text& other); // Copy assignment operator
    ~Text();

    void Update();
    void Draw(const Camera& camera, const Vector2D& screenSize);

    // Setters
    void setMessage(char* message);
    void setMessage(std::string message);
    void setFont(TTF_Font* textFont);
    void setTexture(SDL_Texture* texture);
    void setPosX(int posX);
    void setPosY(int posY);
    void setIsFixed(bool fixed);

    // Getters
    char* getMessage();
    TTF_Font* getFont();
    SDL_Texture* getTexture();
    int getPosX();
    int getPosY();
    bool getIsFixed();
};