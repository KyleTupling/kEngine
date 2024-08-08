#pragma once

#include "UIElement.h"
#include "Vector2D.h"
#include <SDL_ttf.h>
#include <string>
#include <functional>

class UIButton : public UIElement
{
public:
	UIButton(int x, int y, int w, int h, const std::string& text, TTF_Font* font, SDL_Color color);
	void Draw(SDL_Renderer* renderer) override;
	void HandleEvent(const SDL_Event& event) override;
	// Set the function that runs if the button is clicked
	void SetOnClick(std::function<void()> callback);

private:
	SDL_Rect rect;
	SDL_Color color;
	SDL_Color hoveredColor = { 100, 100, 200, 255 };
	std::string text;
	TTF_Font* font;
	bool isHovered = false;

	std::function<void()> OnClick; // Store click handle function
};

