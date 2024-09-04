#include "UILabel.h"

UILabel::UILabel(const Renderer& renderer, const Vector2D& position, const std::string& text, TTF_Font* font, SDL_Color textColor, SDL_Color backgroundColor)
	: text(text), font(font), textColor(textColor), backgroundColor(backgroundColor)
{
	m_Position = position;
	rect = { static_cast<int>(m_Position.x), static_cast<int>(m_Position.y), 0, 0 };
	UpdateTexture(renderer);
}

UILabel::~UILabel()
{
	// Clean texture
	if (texture)
	{
		SDL_DestroyTexture(texture);
	}
}

void UILabel::Draw(const Renderer& renderer)
{
	if (TextUpdater)
	{
		TextUpdater();
	}

	if (texture)
	{
		if (m_IsFixedToScreen)
		{
			if (drawBackground)
			{
				renderer.DrawRectOnScreen(m_Position, width + 20, height + 20, backgroundColor);
			}
			renderer.DrawTextureOnScreen(texture, m_Position, width, height);
		}
		else
		{
			renderer.DrawTextureInWorld(texture, m_Position, width, height);
		}
	}
}

void UILabel::HandleEvent(const SDL_Event& event)
{

}

void UILabel::UpdateTexture(const Renderer& renderer)
{
	if (texture)
	{
		SDL_DestroyTexture(texture);
	}

	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
	texture = renderer.CreateTexture(surface);
    SDL_FreeSurface(surface);

	int w, h;
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h); // Get width and height of texture
	rect.w = w;
	rect.h = h;
	width = w;
	height = h;
}

const std::string& UILabel::GetText() const
{
	return text;
}

const SDL_Color& UILabel::GetTextColor() const
{
	return textColor;
}

const SDL_Color& UILabel::GetBackgroundColor() const
{
	return backgroundColor;
}

int UILabel::GetWidth() const
{
	return rect.w;
}

int UILabel::GetHeight() const
{
	return rect.h;
}

bool UILabel::GetDrawBackground() const
{
	return drawBackground;
}

void UILabel::SetText(const Renderer& renderer, const std::string& text)
{
	this->text = text;
	UpdateTexture(renderer);
}

void UILabel::SetTextColor(const SDL_Color& color)
{
	textColor = color;
}

void UILabel::SetBackgroundColor(const SDL_Color& color)
{
	backgroundColor = color;
}

void UILabel::SetDrawBackground(bool drawBG)
{
	drawBackground = drawBG;
}

void UILabel::SetTextUpdater(std::function<void()> callback)
{
	TextUpdater = callback;
}