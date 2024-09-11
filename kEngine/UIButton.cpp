#include "UIButton.h"

extern SDL_Color colorWhite;

UIButton::UIButton(const Renderer& renderer, const Vector2D& position, int w, int h, const std::string& text, TTF_Font* font, SDL_Color color)
	: m_Font(font), m_Color(color)
{
	m_Position = position;
	m_Width = w;
	m_Height = h;

	m_Label = std::make_unique<UILabel>(renderer, m_Position, text, font, colorWhite);
}

void UIButton::Draw(const Renderer& renderer)
{
	m_Label->SetIsFixedToScreen(m_IsFixedToScreen);

	SDL_Color drawColor = m_IsHovered ? m_HoveredColor : m_Color;
	
	if (m_IsFixedToScreen)
	{
		renderer.DrawRectOnScreen(m_Position, m_Width, m_Height, drawColor);
	}
	else
	{
		renderer.DrawRectInWorld(m_Position, m_Width, m_Height, drawColor);
	}
	
	m_Label->Draw(renderer);
}

void UIButton::HandleEvent(const SDL_Event& event)
{
	if (event.type == SDL_MOUSEMOTION)
	{
		//int mouseX, mouseY;
		//SDL_GetMouseState(&mouseX, &mouseY);
		//isHovered = mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h;
	}
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (m_IsHovered)
		{
			if (OnClick)
			{
				OnClick();
			}
		}
	}
}

void UIButton::SetOnClick(std::function<void()> callback)
{
	OnClick = callback;
}

void UIButton::CheckHover(const Vector2D& mousePos, const Renderer& renderer)
{
	if (!m_IsFixedToScreen)
	{
		m_IsHovered = renderer.IsPointInWorldRect(mousePos, m_Position, m_Width, m_Height);
	}
	else
	{
		m_IsHovered = renderer.IsPointInScreenRect(mousePos, m_Position, m_Width, m_Height);
	}
}

void UIButton::SetPosition(const Vector2D& position)
{
	// Translate child elements
	Vector2D difference = position - m_Position;
	m_Label->SetPosition(m_Label->GetPosition() + difference);
	m_Position = position;
}

void UIButton::SetHoveredColor(SDL_Color color)
{
	m_HoveredColor = color;
}