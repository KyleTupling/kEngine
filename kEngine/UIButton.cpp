#include "UIButton.h"

extern SDL_Color colorWhite;

UIButton::UIButton(const Renderer& renderer, const Vector2D& position, int w, int h, const std::string& text, TTF_Font* font, SDL_Color color)
	: font(font), color(color)
{
	m_Position = position;
	width = w;
	height = h;

	label = std::make_unique<UILabel>(renderer, Vector2D(position.x + w / 2, position.y + h / 2 - 10), text, font, colorWhite);
}

void UIButton::Draw(const Renderer& renderer)
{
	label->SetIsFixedToScreen(m_IsFixedToScreen);

	/*if (isHovered)
	{
		SDL_SetRenderDrawColor(renderer, hoveredColor.r, hoveredColor.g, hoveredColor.b, hoveredColor.a);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	}*/

	SDL_Color drawColor = isHovered ? color : hoveredColor;
	
	if (m_IsFixedToScreen)
	{
		renderer.DrawRectOnScreen(m_Position, width, height, drawColor);
	}
	else
	{
		renderer.DrawRectInWorld(m_Position, width, height, drawColor);
	}
	
	label->Draw(renderer);
}

void UIButton::HandleEvent(const SDL_Event& event)
{
	if (event.type == SDL_MOUSEMOTION)
	{
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		//isHovered = mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h;
	}
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (isHovered)
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
		isHovered = renderer.IsPointInWorldRect(mousePos, m_Position, width, height);
	}
	else
	{
		isHovered = renderer.IsPointInScreenRect(mousePos, m_Position, width, height);
	}
}

void UIButton::SetPosition(const Vector2D& position)
{
	Vector2D difference = position - m_Position;
	label->SetPosition(label->GetPosition() + difference);
}