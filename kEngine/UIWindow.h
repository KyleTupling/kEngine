#pragma once

#include "UIElement.h"
#include "UILabel.h"
#include "UIButton.h"
#include <vector>
#include <memory>
#include <string>

class UIWindow : public UIElement
{
public:
	UIWindow(const Renderer& renderer, const Vector2D& position, int width, int height, const std::string& title, TTF_Font* font);
	~UIWindow();

	void Draw(const Renderer& renderer) override;
	void HandleEvent(const SDL_Event& event) override;
	// Closes the window (stops displaying)
	void Close();

	/**
	 * @brief Adds a child UIElement to the UIWindow.
	 * These elements will be rendered automatically with the UIWindow.
	 *
	 * @param element Unique pointer to the UIElement
	 */
	void AddUIElement(std::unique_ptr<UIElement> element);

	int GetWidth() const;
	int GetHeight() const;
	const SDL_Color& GetBackgroundColor() const;
	bool GetIsDisplayed() const;

	void SetPosition(const Vector2D& position);
	void SetWidth(int w);
	void SetHeight(int h);
	void SetBackgroundColor(const SDL_Color& color);
	void SetIsDisplayed(bool isDisplayed);
	void SetIsFixedToScreen(bool isFixed) override;

	void CheckHover(const Vector2D& mousePos, const Renderer& renderer) override;

private:
	int width;
	int height;

	SDL_Color m_BackgroundColor = { 0, 0, 0, 255 };

	std::unique_ptr<UILabel> windowTitleLabel; // Does this need to be a unique pointer?
	bool drawTitle = true;

	std::unique_ptr<UIButton> windowCloseButton;
	bool drawCloseButton = true;

	bool isDisplayed = true;

	std::vector<std::unique_ptr<UIElement>> childrenElements;
};

