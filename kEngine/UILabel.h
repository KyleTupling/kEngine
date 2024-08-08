#pragma once

#include "UIElement.h"
#include <SDL_ttf.h>
#include <string>

class UILabel : public UIElement
{
public:
	UILabel(SDL_Renderer* renderer, int x, int y, const std::string& text, TTF_Font* font, SDL_Color textColor, SDL_Color backgroundColor = { 0, 0, 0, 0 });
	~UILabel();
	void Draw(SDL_Renderer* renderer) override;
	void HandleEvent(const SDL_Event& event) override;
	void UpdateTexture();

	int GetPosX() const;
	int GetPosY() const;
	int GetWidth() const;
	int GetHeight() const;

	void SetPosition(int x, int y);
	void SetText(const std::string& text);

private:
	std::string text;
	TTF_Font* font;
	SDL_Color textColor;
	SDL_Color backgroundColor;

	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Rect rect;
};

