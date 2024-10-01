#include "UIWindow.h"
#include "Graphics/Color.h"

UIWindow::UIWindow(const Renderer& renderer, const Vector2D& position, int width, int height, const std::string& title, TTF_Font* font)
	: width(width), height(height)
{
	m_Position = position;

	// Does this need to be a unique pointer?
	windowTitleLabel = std::make_unique<UILabel>(renderer, Vector2D(m_Position.x, m_Position.y - height / 2 + 15), title, font, Color::White, Color::Black);
	
	// Create close button
	windowCloseButton = std::make_unique<UIButton>(renderer, m_Position + Vector2D(width / 2 - 15, 15 - height / 2), 30, 30, "X", font, Color::Black);
	windowCloseButton->SetHoveredColor({ 255, 0, 0, 255 });
	windowCloseButton->SetOnClick([this]() 
		{
			this->Close(); 
			windowCloseButton->SetIsHovered(false);
		});
}

UIWindow::~UIWindow()
{
	// Smart pointers used (handle memory deallocation automatically)
}

void UIWindow::Draw(const Renderer& renderer)
{
	if (m_IsVisible)
	{
		if (m_IsFixedToScreen)
		{
			renderer.DrawRectOnScreen(m_Position, width, height, m_BackgroundColor);

			if (drawTitle) // Draw window titlebar
			{
				renderer.DrawRectOnScreen(windowTitleLabel->GetPosition(), width, 30, {50, 50, 50, 255});
				windowTitleLabel->Draw(renderer);
			}
		}
		else
		{
			renderer.DrawRectInWorld(m_Position, width, height, m_BackgroundColor);

			if (drawTitle) // Draw window titlebar
			{
				renderer.DrawRectInWorld(windowTitleLabel->GetPosition(), width, 30, {50, 50, 50, 255});
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
	if (m_IsVisible) // Only handle window events if the window is being displayed
	{
		if (drawCloseButton) windowCloseButton->HandleEvent(event);

		// Handle child element events
		for (const auto& element : childrenElements)
		{
			if (element->GetIsVisible())
			{
				element->HandleEvent(event);
			}
		}
	}
}

void UIWindow::Close()
{
	m_IsVisible = false;
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

const SDL_Color& UIWindow::GetBackgroundColor() const
{
	return m_BackgroundColor;
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
	
	m_Position = position;
}

void UIWindow::SetWidth(int w)
{
	width = w;
}

void UIWindow::SetHeight(int h)
{
	height = h;
}

void UIWindow::SetBackgroundColor(const SDL_Color& color)
{
	m_BackgroundColor = color;
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

void UIWindow::SetDrawTitle(bool shouldDraw)
{
	drawTitle = shouldDraw;
}

void UIWindow::SetDrawCloseButton(bool shouldDraw)
{
	drawCloseButton = shouldDraw;
}

void UIWindow::SetDrawTitleBar(bool shouldDraw)
{
	SetDrawTitle(shouldDraw);
	SetDrawCloseButton(shouldDraw);
}