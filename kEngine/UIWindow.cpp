#include "UIWindow.h"

extern SDL_Color colorWhite;
extern SDL_Color colorBlack;

UIWindow::UIWindow(SDL_Renderer* renderer, int posX, int posY, int width, int height, const std::string& title, TTF_Font* font)
	: width(width), height(height)
{
	m_posX = posX;
	m_posY = posY;

	// Does this need to be a unique pointer?
	windowTitleLabel = std::make_unique<UILabel>(renderer, m_posX + width / 2, m_posY + 5, title, font, colorWhite, colorBlack);
	
	// Create close button
	windowCloseButton = std::make_unique<UIButton>(renderer, m_posX + width - 30, m_posY, 30, 30, "X", font, colorBlack);
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
		if (m_isFixedToScreen)
		{
			SDL_Rect rect = { m_posX, m_posY, width, height };
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderFillRect(renderer, &rect);

			if (drawTitle) // Draw window titlebar
			{
				rect = { m_posX, m_posY, width, 30 };
				SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
				SDL_RenderFillRect(renderer, &rect);
				windowTitleLabel->Draw(renderer, camera, screenSize);
			}
		}
		else
		{
			Vector2D screenPos = camera.ConvertWorldToScreen(Vector2D(m_posX, m_posY), screenSize);
			SDL_Rect rect = { screenPos.x, screenPos.y, width * camera.zoom, height * camera.zoom };
			SDL_RenderFillRect(renderer, &rect);

			if (drawTitle) // Draw window titlebar
			{
				rect = { 
					static_cast<int>(screenPos.x), 
					static_cast<int>(screenPos.y), 
					static_cast<int>(width * camera.zoom),
					static_cast < int>(30 * camera.zoom)
				};
				SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
				SDL_RenderFillRect(renderer, &rect);
				windowTitleLabel->Draw(renderer, camera, screenSize);
			}
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
	element->SetIsFixedToScreen(m_isFixedToScreen);
	childrenElements.push_back(std::move(element));
}

void UIWindow::CheckHover(const Vector2D& mousePos, const Camera& camera, const Vector2D& screenSize)
{
	if (drawCloseButton) windowCloseButton->CheckHover(mousePos, camera, screenSize);

	for (const auto& element : childrenElements)
	{
		element->CheckHover(mousePos, camera, screenSize);
	}
}


// Getters
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

void UIWindow::SetPosX(int x)
{
	int posDifference = x - m_posX;
	UIElement::SetPosX(x);

	int currentCloseBtnPosX = windowCloseButton->GetPosX();
	windowCloseButton->SetPosX(currentCloseBtnPosX + posDifference);

	int currentLabelPosX = windowTitleLabel->GetPosX();
	windowTitleLabel->SetPosX(currentLabelPosX + posDifference);

	for (const auto& element : childrenElements)
	{
		int currentChildPosX = element->GetPosX();
		element->SetPosX(currentChildPosX + posDifference);
	}
}

void UIWindow::SetPosY(int y)
{
	int posDifference = y - m_posY;
	UIElement::SetPosY(y);

	int currentCloseBtnPosY = windowCloseButton->GetPosY();
	windowCloseButton->SetPosY(currentCloseBtnPosY + posDifference);

	int currentLabelPosY = windowTitleLabel->GetPosY();
	windowTitleLabel->SetPosY(currentLabelPosY + posDifference);

	for (const auto& element : childrenElements)
	{
		int currentChildPosY = element->GetPosY();
		element->SetPosY(currentChildPosY + posDifference);
	}
}

void UIWindow::SetWidth(int w)
{
	width = w;
}

void UIWindow::SetHeight(int h)
{
	height = h;
}

void UIWindow::SetIsDisplayed(bool isDisplayed)
{
	this->isDisplayed = isDisplayed;
}
void UIWindow::SetIsFixedToScreen(bool isFixed)
{
	UIElement::SetIsFixedToScreen(isFixed);
	windowTitleLabel->SetIsFixedToScreen(m_isFixedToScreen);
	windowCloseButton->SetIsFixedToScreen(m_isFixedToScreen);
	
	// Apply same attribute to children elements
	for (const auto& element : childrenElements)
	{
		element->SetIsFixedToScreen(isFixed);
	}
}