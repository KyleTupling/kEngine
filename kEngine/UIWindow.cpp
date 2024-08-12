#include "UIWindow.h"

extern SDL_Color colorWhite;
extern SDL_Color colorBlack;

UIWindow::UIWindow(SDL_Renderer* renderer, int posX, int posY, int width, int height, const std::string& title, TTF_Font* font)
	: posX(posX), posY(posY), width(width), height(height)
{
	// Does this need to be a unique pointer?
	windowTitleLabel = std::make_unique<UILabel>(renderer, posX + width / 2, posY + 5, title, font, colorWhite, colorBlack);
	
	// Create close button
	windowCloseButton = std::make_unique<UIButton>(posX + width - 30, posY, 30, 30, "X", font, colorBlack);
	windowCloseButton->SetOnClick([this]() 
		{
			this->Close(); 
		});
}

UIWindow::~UIWindow()
{
	// Smart pointers used (handle memory deallocation automatically)
}

void UIWindow::Draw(SDL_Renderer* renderer, const Camera& camera, const Vector2D& screenSize)
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
			windowTitleLabel->Draw(renderer, camera, screenSize);
		}

		if (drawCloseButton)
		{
			windowCloseButton->Draw(renderer, camera, screenSize);
		}

		for (const auto& element : childrenElements)
		{
			element->Draw(renderer, camera, screenSize);
		}
	}
}

void UIWindow::HandleEvent(const SDL_Event& event)
{
	if (isDisplayed) // Only handle window events if the window is being displayed
	{
		if (drawCloseButton) windowCloseButton->HandleEvent(event);

		// Handle child element events
		for (const auto& element : childrenElements)
		{
			element->HandleEvent(event);
		}
	}
}

void UIWindow::Close()
{
	isDisplayed = false;
}

void UIWindow::AddUIElement(std::unique_ptr<UIElement> element)
{
	childrenElements.push_back(std::move(element));
}


// Getters

int UIWindow::GetPosX() const
{
	return posX;
}

int UIWindow::GetPosY() const
{
	return posY;
}

int UIWindow::GetWidth() const
{
	return width;
}

int UIWindow::GetHeight() const
{
	return height;
}

bool UIWindow::GetIsDisplayed() const
{
	return isDisplayed;
}

// Setters

void UIWindow::SetIsDisplayed(bool isDisplayed)
{
	this->isDisplayed = isDisplayed;
}