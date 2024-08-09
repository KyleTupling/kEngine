#include "UIButton.h"

UIButton::UIButton(int x, int y, int w, int h, const std::string& text, TTF_Font* font, SDL_Color color)
	: rect{ x, y, w, h }, text(text), font(font), color(color) {}

void UIButton::Draw(SDL_Renderer* renderer)
{
	if (isHovered)
	{
		SDL_SetRenderDrawColor(renderer, hoveredColor.r, hoveredColor.g, hoveredColor.b, hoveredColor.a);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	}
	
	SDL_RenderFillRect(renderer, &rect);

	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), { 255, 255, 255, 255 });
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect textRect = { rect.x + rect.w / 2 - surface->w / 2, rect.y + rect.h / 2 - surface->h / 2, surface->w, surface->h };
	SDL_RenderCopy(renderer, texture, NULL, &textRect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
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