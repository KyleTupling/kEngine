#pragma once

#include "UIElement.h"
#include "Vector2D.h"
#include <SDL_ttf.h>
#include <string>

class UIButton : public UIElement
{
public:
	UIButton(int x, int y, int w, int h, const std::string& text, TTF_Font* font, SDL_Color color);
	void Draw(SDL_Renderer* renderer) override;
	void HandleEvent(const SDL_Event& event) override;

private:
	SDL_Rect rect;
	SDL_Color color;
	SDL_Color hoveredColor = { 100, 100, 200, 255 };
	std::string text;
	TTF_Font* font;
	bool isHovered = false;
};

