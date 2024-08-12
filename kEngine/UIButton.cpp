#include "UIButton.h"

extern SDL_Color colorWhite;

UIButton::UIButton(SDL_Renderer* renderer, int x, int y, int w, int h, const std::string& text, TTF_Font* font, SDL_Color color)
	: rect{ x, y, w, h }, font(font), color(color)
{
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
	
	// TODO: Refactor to avoid creating and destroying surface and texture every frame
	if (m_isFixedToScreen)
	{
		SDL_RenderFillRect(renderer, &rect);
	}
	else
	{
		Vector2D screenPos = camera.ConvertWorldToScreen(Vector2D(rect.x, rect.y), screenSize);
		SDL_Rect drawRect = { 
			screenPos.x, 
			screenPos.y,
			rect.w * camera.zoom, 
			rect.h * camera.zoom 
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

		isHovered = mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h;
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