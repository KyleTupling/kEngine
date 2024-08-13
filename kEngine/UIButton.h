#pragma once

#include "UIElement.h"
#include "UILabel.h"
#include "Vector2D.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <functional>
#include <memory>

class UIButton : public UIElement
{
public:
	UIButton(SDL_Renderer* renderer, int x, int y, int w, int h, const std::string& text, TTF_Font* font, SDL_Color color);
	void Draw(SDL_Renderer* renderer, const Camera& camera, const Vector2D& screenSize) override;
	void HandleEvent(const SDL_Event& event) override;

	// Set the function that runs if the button is clicked
	void SetOnClick(std::function<void()> callback);

	void CheckHover(const Vector2D& mousePos, const Camera& camera, const Vector2D& screenSize) override;
	
	void SetPosX(int x) override;
	void SetPosY(int y) override;

private:
	int width;
	int height;
	SDL_Color color; 
	SDL_Color hoveredColor = { 100, 100, 200, 255 };
	std::unique_ptr<UILabel> label;
	TTF_Font* font;
	bool isHovered = false;

	std::function<void()> OnClick; // Store click handle function
};

