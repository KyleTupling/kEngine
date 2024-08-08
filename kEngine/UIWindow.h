#pragma once

#include "UIElement.h"
#include "UILabel.h"
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

	void AddUIElement(std::unique_ptr<UIElement> element);

private:
	int posX;
	int posY;
	int width;
	int height;

	std::unique_ptr<UILabel> windowTitleLabel;
	bool drawTitle = true;

	bool isDisplayed = true;

	std::vector<std::unique_ptr<UIElement>> childrenElements;
};

