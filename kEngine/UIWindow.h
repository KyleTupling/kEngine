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
	UIWindow(SDL_Renderer* renderer, int posX, int posY, int width, int height, const std::string& title, TTF_Font* font);
	~UIWindow();

	void Draw(SDL_Renderer* renderer) override;
	void HandleEvent(const SDL_Event& event) override;
	// Closes the window (stops displaying)
	void Close();

	// Add child UIElement
	void AddUIElement(std::unique_ptr<UIElement> element);

	int GetPosX() const;
	int GetPosY() const;
	int GetWidth() const;
	int GetHeight() const;
	bool GetIsDisplayed() const;

	void SetIsDisplayed(bool isDisplayed);

private:
	int posX;
	int posY;
	int width;
	int height;

	std::unique_ptr<UILabel> windowTitleLabel; // Does this need to be a unique pointer?
	bool drawTitle = true;

	std::unique_ptr<UIButton> windowCloseButton;
	bool drawCloseButton = true;

	bool isDisplayed = true;

	std::vector<std::unique_ptr<UIElement>> childrenElements;
};

