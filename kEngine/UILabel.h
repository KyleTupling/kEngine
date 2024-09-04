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
	UILabel(const Renderer& renderer, const Vector2D& position, const std::string& text, TTF_Font* font, SDL_Color textColor, SDL_Color backgroundColor = { 0, 0, 0, 0 });
	~UILabel();

	void Draw(const Renderer& renderer) override;
	void HandleEvent(const SDL_Event& event) override;
	void UpdateTexture(const Renderer& renderer);

	int GetWidth() const;
	int GetHeight() const;
	const std::string& GetText() const;
	const SDL_Color& GetTextColor() const;
	const SDL_Color& GetBackgroundColor() const;
	bool GetDrawBackground() const;

	void SetText(const Renderer& renderer, const std::string& text);
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
	int width;
	int height;
	std::string text;
	TTF_Font* font;
	SDL_Color textColor;
	SDL_Color backgroundColor;
	bool drawBackground = false;

	SDL_Rect rect;
	SDL_Texture* texture;

	std::function<void()> TextUpdater;
};

