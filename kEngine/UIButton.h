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

	void CheckHover(const Vector2D& mousePos, const Renderer& renderer) override;

	// Overrides UIElement as needs to propogate position change to all children
	void SetPosition(const Vector2D& position) override;

	/**
	 * Sets the function that runs when button is clicked.
	 *
	 * @param callback The callback function to run on click.
	 * 
	 * @note This currently seems to work best with lambda functions, due for a refactor.
	 */
	void SetOnClick(std::function<void()> callback);

	/**
	 * Gets the UILabel object used to show text on the button.
	 *
	 * @return A read-only reference to the label.
	 */
	const UILabel& GetLabel() const;

	void SetText(const Renderer& renderer, const std::string& text);

	int GetWidth() const;
	void SetWidth(int width);

	int GetHeight() const;
	void SetHeight(int height);

	const SDL_Color& GetColor() const;
	void SetColor(const SDL_Color& color);

	const SDL_Color& GetHoveredColor() const;
	void SetHoveredColor(const SDL_Color& color);

	bool GetIsHovered() const;
	void SetIsHovered(bool isHovered);

private:
	int m_Width;
	int m_Height;
	SDL_Color m_Color = { 0, 0, 0, 255 };
	SDL_Color m_HoveredColor = { 200, 70, 160, 255 };
	std::unique_ptr<UILabel> m_Label;
	bool m_IsHovered = false;

	std::function<void()> OnClick; // Store click handle function
};

