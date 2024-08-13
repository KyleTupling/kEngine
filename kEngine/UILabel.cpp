#include "UILabel.h"

UILabel::UILabel(SDL_Renderer* renderer, int x, int y, const std::string& text, TTF_Font* font, SDL_Color textColor, SDL_Color backgroundColor)
	: text(text), font(font), textColor(textColor), backgroundColor(backgroundColor), renderer(renderer), rect{ x, y, 0, 0 }
{
	m_posX = x;
	m_posY = y;
	UpdateTexture();
}

UILabel::~UILabel()
{
	// Clean texture
	if (texture)
	{
		SDL_DestroyTexture(texture);
	}
}

void UILabel::Draw(SDL_Renderer* renderer, const Camera& camera, const Vector2D& screenSize)
{
	if (TextUpdater)
	{
		TextUpdater();
	}

	if (texture)
	{
		if (m_isFixedToScreen)
		{
			if (drawBackground)
			{
				SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
				SDL_Rect backgroundRect = { m_posX - width / 2 - 10, m_posY + height / 2 - 10 - 10, width + 20, height + 20 };
				SDL_RenderFillRect(renderer, &backgroundRect);
			}
			SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
			SDL_Rect drawRect = { m_posX - width / 2, m_posY + height / 2 - 10, width, height };
			SDL_RenderCopy(renderer, texture, nullptr, &drawRect);
		}
		else
		{
			Vector2D screenPos = camera.ConvertWorldToScreen(Vector2D(m_posX, m_posY), screenSize);
			SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
			SDL_Rect drawRect = { screenPos.x -  (width / 2) * camera.zoom, screenPos.y + (height / 2 - 10) * camera.zoom, width * camera.zoom, height * camera.zoom };
			SDL_RenderCopy(renderer, texture, nullptr, &drawRect);
		}
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

void UILabel::SetPosition(int x, int y)
{
	UIElement::SetPosX(x);
	UIElement::SetPosY(y);
}

void UILabel::SetText(const std::string& text)
{
	this->text = text;
	UpdateTexture();
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