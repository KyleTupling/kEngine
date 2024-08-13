#pragma once

#include "UIElement.h"
#include "Camera.h"
#include <SDL_ttf.h>
#include <string>
#include <memory>
#include <functional>

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
	const std::string& GetText() const;
	const SDL_Color& GetTextColor() const;
	const SDL_Color& GetBackgroundColor() const;
	bool GetDrawBackground() const;

	void SetPosition(int x, int y);
	void SetText(const std::string& text);
	void SetTextColor(const SDL_Color& color);
	void SetBackgroundColor(const SDL_Color& color);

	/**
	 * @brief Sets whether to draw the background of the label or not.
	 *
	 * @param drawBG Whether to draw the background
	 */
	void SetDrawBackground(bool drawBG);

	void SetTextUpdater(std::function<void()> callback);

private:
	std::string text;
	TTF_Font* font;
	SDL_Color textColor;
	SDL_Color backgroundColor;
	bool drawBackground = false;

	SDL_Rect rect;
	SDL_Renderer* renderer;
	SDL_Texture* texture;

	std::function<void()> TextUpdater;
};

