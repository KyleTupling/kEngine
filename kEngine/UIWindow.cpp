#include "UIWindow.h"

extern SDL_Color colorWhite;
extern SDL_Color colorBlack;

UIWindow::UIWindow(const Renderer& renderer, const Vector2D& position, int width, int height, const std::string& title, TTF_Font* font)
	: width(width), height(height)
{
	m_Position = position;

	// Does this need to be a unique pointer?
	windowTitleLabel = std::make_unique<UILabel>(renderer, Vector2D(m_Position.x + width / 2, m_Position.y + 5), title, font, colorWhite, colorBlack);
	
	// Create close button
	windowCloseButton = std::make_unique<UIButton>(renderer, Vector2D(m_Position.x + width - 30, m_Position.y), 30, 30, "X", font, colorBlack);
	windowCloseButton->SetOnClick([this]() 
		{
			this->Close(); 
		});
}

UIWindow::~UIWindow()
{
	// Smart pointers used (handle memory deallocation automatically)
}

void UIWindow::Draw(const Renderer& renderer)
{
	if (isDisplayed)
	{
		if (m_IsFixedToScreen)
		{
			renderer.DrawRectOnScreen(m_Position, width, height, { 0, 0, 0, 255 });

			if (drawTitle) // Draw window titlebar
			{
				renderer.DrawRectOnScreen(m_Position, width, 30, { 50, 50, 50, 255 });
				windowTitleLabel->Draw(renderer);
			}
		}
		else
		{
			renderer.DrawRectInWorld(m_Position, width, height, { 0, 0, 0, 255 });

			if (drawTitle) // Draw window titlebar
			{
				renderer.DrawRectInWorld(m_Position, width, 30, { 50, 50, 50, 255 });
				windowTitleLabel->Draw(renderer);
			}
		}

		if (drawCloseButton)
		{
			windowCloseButton->Draw(renderer);
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
	element->SetIsFixedToScreen(m_IsFixedToScreen);
	childrenElements.push_back(std::move(element));
}

void UIWindow::CheckHover(const Vector2D& mousePos, const Renderer& renderer)
{
	if (drawCloseButton) windowCloseButton->CheckHover(mousePos, renderer);

	for (const auto& element : childrenElements)
	{
		element->CheckHover(mousePos, renderer);
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

void UIWindow::SetPosition(const Vector2D& position)
{
	// Translate all child elements
	Vector2D difference = position - m_Position;
	
	windowCloseButton->SetPosition(windowCloseButton->GetPosition() + difference);

	windowTitleLabel->SetPosition(windowTitleLabel->GetPosition() + difference);

	for (const auto& element : childrenElements)
	{
		element->SetPosition(element->GetPosition() + difference);
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
	windowTitleLabel->SetIsFixedToScreen(m_IsFixedToScreen);
	windowCloseButton->SetIsFixedToScreen(m_IsFixedToScreen);
	
	// Apply same attribute to children elements
	for (const auto& element : childrenElements)
	{
		element->SetIsFixedToScreen(isFixed);
	}
}