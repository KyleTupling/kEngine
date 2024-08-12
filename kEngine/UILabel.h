#pragma once

#include "UIElement.h"
#include "Camera.h"
#include <SDL_ttf.h>
#include <string>
#include <memory>

class UILabel : public UIElement
{
public:
	UILabel(SDL_Renderer* renderer, int x, int y, const std::string& text, TTF_Font* font, SDL_Color textColor, SDL_Color backgroundColor = { 0, 0, 0, 0 });
	~UILabel();
	void Draw(SDL_Renderer* renderer, const Camera& camera, const Vector2D& screenSize) override;
	void HandleEvent(const SDL_Event& event) override;
	void UpdateTexture();

	int GetPosX() const;
	int GetPosY() const;
	int GetWidth() const;
	int GetHeight() const;
	bool GetDrawBackground() const;

	void SetPosition(int x, int y);
	void SetText(const std::string& text);
	void SetDrawBackground(bool drawBG);

private:
	std::string text;
	TTF_Font* font;
	SDL_Color textColor;
	SDL_Color backgroundColor;
	bool drawBackground = false;

	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Rect rect;
};

