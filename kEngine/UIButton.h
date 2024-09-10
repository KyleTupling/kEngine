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
	UIButton(const Renderer& renderer, const Vector2D& position, int w, int h, const std::string& text, TTF_Font* font, SDL_Color color);
	void Draw(const Renderer& renderer) override;
	void HandleEvent(const SDL_Event& event) override;

	// Set the function that runs if the button is clicked
	void SetOnClick(std::function<void()> callback);

	void CheckHover(const Vector2D& mousePos, const Renderer& renderer) override;
	
	void SetPosition(const Vector2D& position) override;
	void SetHoveredColor(SDL_Color color);

private:
	int width;
	int height;
	SDL_Color color; 
	SDL_Color m_HoveredColor = { 200, 70, 160, 255 };
	std::unique_ptr<UILabel> label;
	TTF_Font* font;
	bool isHovered = false;

	std::function<void()> OnClick; // Store click handle function
};

