#include "UIWindow.h"

extern SDL_Color colorWhite;
extern SDL_Color colorBlack;

UIWindow::UIWindow(SDL_Renderer* renderer, int posX, int posY, int width, int height, const std::string& title, TTF_Font* font)
	: posX(posX), posY(posY), width(width), height(height)
{
	windowTitleLabel = std::make_unique<UILabel>(renderer, posX + width / 2, posY + 5, title, font, colorWhite, colorBlack);
}

UIWindow::~UIWindow()
{
	// Smart pointers used (handle memory deallocation automatically)
}

void UIWindow::Draw(SDL_Renderer* renderer)
{
	if (isDisplayed)
	{
		SDL_Rect rect = { posX, posY, width, height };
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &rect);

		if (drawTitle) // Draw window titlebar
		{
			rect = { posX, posY, width, 30 };
			SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
			SDL_RenderFillRect(renderer, &rect);
			windowTitleLabel->Draw(renderer);
		}

		for (const auto& element : childrenElements)
		{
			element->Draw(renderer);
		}
	}
}

void UIWindow::HandleEvent(const SDL_Event& event)
{
	if (isDisplayed) // Only handle window events if the window is being displayed
	{
		// Handle child element events
		for (const auto& element : childrenElements)
		{
			element->HandleEvent(event);
		}
	}
}

void UIWindow::AddUIElement(std::unique_ptr<UIElement> element)
{
	childrenElements.push_back(std::move(element));
}