#include "UIButton.h"

extern SDL_Color colorWhite;

UIButton::UIButton(SDL_Renderer* renderer, int x, int y, int w, int h, const std::string& text, TTF_Font* font, SDL_Color color)
	: font(font), color(color)
{
	m_posX = x;
	m_posY = y;
	width = w;
	height = h;

	label = std::make_unique<UILabel>(renderer, x + w / 2, y + h / 2 - 10, text, font, colorWhite);
}

void UIButton::Draw(SDL_Renderer* renderer, const Camera& camera, const Vector2D& screenSize)
{
	label->SetIsFixedToScreen(m_isFixedToScreen);

	if (isHovered)
	{
		SDL_SetRenderDrawColor(renderer, hoveredColor.r, hoveredColor.g, hoveredColor.b, hoveredColor.a);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	}
	
	if (m_isFixedToScreen)
	{
		SDL_Rect rect{ m_posX, m_posY, width, height };
		SDL_RenderFillRect(renderer, &rect);
	}
	else
	{
		Vector2D screenPos = camera.ConvertWorldToScreen(Vector2D(m_posX, m_posY), screenSize);
		SDL_Rect drawRect = { 
			screenPos.x, 
			screenPos.y,
			width * camera.zoom, 
			height * camera.zoom 
		};
		SDL_RenderFillRect(renderer, &drawRect);
	}
	
	label->Draw(renderer, camera, screenSize);
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

void UIButton::CheckHover(const Vector2D& mousePos, const Camera& camera, const Vector2D& screenSize)
{
	if (!m_isFixedToScreen)
	{
		Vector2D screenPos = camera.ConvertWorldToScreen(Vector2D(m_posX, m_posY), screenSize);
		isHovered = mousePos.x >= screenPos.x && mousePos.x <= screenPos.x + width * camera.zoom && mousePos.y >= screenPos.y && mousePos.y <= screenPos.y + height * camera.zoom;
	}
	else
	{
		isHovered = mousePos.x >= m_posX && mousePos.x <= m_posX + width && mousePos.y >= m_posY && mousePos.y <= m_posY + height;
	}
}

void UIButton::SetPosX(int x)
{
	int difference = x - m_posX;
	label->SetPosX(label->GetPosX() + difference);
	UIElement::SetPosX(x);
}

void UIButton::SetPosY(int y)
{
	int difference = y - m_posY;
	label->SetPosY(label->GetPosY() + difference);
	UIElement::SetPosY(y);
}