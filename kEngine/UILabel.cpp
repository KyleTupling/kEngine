#include "UILabel.h"

UILabel::UILabel(SDL_Renderer* renderer, int x, int y, const std::string& text, TTF_Font* font, SDL_Color textColor, SDL_Color backgroundColor)
	: text(text), font(font), textColor(textColor), backgroundColor(backgroundColor), renderer(renderer), rect{ x, y, 0, 0 }
{
	UpdateTexture();
}

UILabel::~UILabel()
{
	// Clean textures
	if (texture)
	{
		SDL_DestroyTexture(texture);
	}
}

void UILabel::Draw(SDL_Renderer* renderer)
{
	if (texture)
	{
		SDL_Rect drawRect = { rect.x - rect.w / 2, rect.y + rect.h / 2 - 10, rect.w, rect.h };
		SDL_RenderCopy(renderer, texture, nullptr, &drawRect);
	}
}

void UILabel::HandleEvent(const SDL_Event& event)
{

}

void UILabel::UpdateTexture()
{
	if (texture)
	{
		SDL_DestroyTexture(texture);
	}

	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

	int w, h;
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
	rect.w = w;
	rect.h = h;
}

int UILabel::GetPosX() const
{
	return rect.x;
}

int UILabel::GetPosY() const
{
	return rect.y;
}

void UILabel::SetPosition(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void UILabel::SetText(const std::string& text)
{
	this->text = text;
	UpdateTexture();
}