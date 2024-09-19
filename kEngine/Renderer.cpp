#include "Renderer.h"

#include <SDL_ttf.h>

Renderer::Renderer(SDL_Window* window, const Vector2D& screenSize, Camera* camera)
	: m_ScreenSize(screenSize), m_Camera(camera)
{
	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_Renderer)
	{
		throw std::runtime_error("Failed to create SDL Renderer");
	}
}

Renderer::~Renderer()
{
	if (m_Renderer)
	{
		SDL_DestroyRenderer(m_Renderer);
	}
}

void Renderer::Clear(SDL_Color color) const
{
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);
}

void Renderer::Present() const
{
	SDL_RenderPresent(m_Renderer);
}

void Renderer::DrawLineInWorld(const Vector2D& worldPosStart, const Vector2D& worldPosEnd, SDL_Color color) const
{
	Vector2D screenPosStart = m_Camera->ConvertWorldToScreen(worldPosStart, m_ScreenSize);
	Vector2D screenPosEnd = m_Camera->ConvertWorldToScreen(worldPosEnd, m_ScreenSize);
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(m_Renderer, screenPosStart.x, screenPosStart.y, screenPosEnd.x, screenPosEnd.y);
}

void Renderer::DrawLineOnScreen(const Vector2D& screenPosStart, const Vector2D& screenPosEnd, SDL_Color color) const
{
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(m_Renderer, screenPosStart.x, screenPosStart.y, screenPosEnd.x, screenPosEnd.y);
}

void Renderer::DrawRectInWorld(const Vector2D& worldPos, int width, int height, SDL_Color color) const
{
	Vector2D screenPos = m_Camera->ConvertWorldToScreen(worldPos, m_ScreenSize);

	SDL_Rect rect = {
		static_cast<int>(screenPos.x - (width / 2) * m_Camera->zoom),
		static_cast<int>(screenPos.y - (height / 2) * m_Camera->zoom),
		static_cast<int>(width * m_Camera->zoom),
		static_cast<int>(height * m_Camera->zoom)
	};

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(m_Renderer, &rect);
}

void Renderer::DrawRectOnScreen(const Vector2D& screenPos, int width, int height, SDL_Color color) const
{
	SDL_Rect rect = {
			static_cast<int>((screenPos.x - width / 2)),
			static_cast<int>((screenPos.y - height / 2)),
			static_cast<int>(width),
			static_cast<int>(height)
	};

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(m_Renderer, &rect);
}

void Renderer::DrawCircleOnScreen(const Vector2D& screenPos, int radius, SDL_Color color) const
{
	int x = radius;
	int y = 0;
	int radiusError = 1 - x;

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);

	while (x >= y) {
		// Draw points in all eight octants
		SDL_RenderDrawPoint(m_Renderer, screenPos.x + x, screenPos.y + y);
		SDL_RenderDrawPoint(m_Renderer, screenPos.x - x, screenPos.y + y);
		SDL_RenderDrawPoint(m_Renderer, screenPos.x + x, screenPos.y - y);
		SDL_RenderDrawPoint(m_Renderer, screenPos.x - x, screenPos.y - y);
		SDL_RenderDrawPoint(m_Renderer, screenPos.x + y, screenPos.y + x);
		SDL_RenderDrawPoint(m_Renderer, screenPos.x - y, screenPos.y + x);
		SDL_RenderDrawPoint(m_Renderer, screenPos.x + y, screenPos.y - x);
		SDL_RenderDrawPoint(m_Renderer, screenPos.x - y, screenPos.y - x);

		++y;
		if (radiusError < 0) {
			radiusError += 2 * y + 1;
		}
		else {
			--x;
			radiusError += 2 * (y - x + 1);
		}
	}
}

void Renderer::DrawTextureInWorld(SDL_Texture* texture, const Vector2D& worldPos, int width, int height) const
{
	Vector2D screenPos = m_Camera->ConvertWorldToScreen(worldPos, m_ScreenSize);

	SDL_Rect rect = {
		static_cast<int>(screenPos.x - (width / 2) * m_Camera->zoom),
		static_cast<int>(screenPos.y - (height / 2) * m_Camera->zoom),
		static_cast<int>(width * m_Camera->zoom),
		static_cast<int>(height * m_Camera->zoom)
	};

	SDL_RenderCopy(m_Renderer, texture, NULL, &rect);
}

void Renderer::DrawTextureOnScreen(SDL_Texture* texture, const Vector2D& screenPos, int width, int height) const
{
	SDL_Rect rect = {
			static_cast<int>(screenPos.x - (width / 2)),
			static_cast<int>(screenPos.y - (height / 2)),
			static_cast<int>(width),
			static_cast<int>(height)
	};
	SDL_RenderCopy(m_Renderer, texture, NULL, &rect);
}

SDL_Texture* Renderer::CreateTexture(SDL_Surface* surface) const
{
	return SDL_CreateTextureFromSurface(m_Renderer, surface);
}

void Renderer::DrawTextOnScreen(const Vector2D& screenPos, const std::string& text, TTF_Font* font, const SDL_Color& color = {255, 255, 255, 255}, bool centered = true) const
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
	SDL_Texture* texture = CreateTexture(surface);
	int w, h;
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
	SDL_Rect textRect;
	if (centered)
	{
		textRect = {
			static_cast<int>(screenPos.x) - w / 2,
			static_cast<int>(screenPos.y) - h / 2,
			w,
			h
		};
	}
	else
	{
		textRect = {
			static_cast<int>(screenPos.x),
			static_cast<int>(screenPos.y),
			w,
			h
		};
	}
	SDL_RenderCopy(m_Renderer, texture, NULL, &textRect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

bool Renderer::IsPointInWorldRect(const Vector2D& point, const Vector2D& rectPos, int rectWidth, int rectHeight) const
{
	Vector2D rectScreenPos = m_Camera->ConvertWorldToScreen(rectPos, m_ScreenSize);
	return point.x >= rectScreenPos.x - (rectWidth / 2) * m_Camera->zoom && point.x <= rectScreenPos.x + (rectWidth / 2) * m_Camera->zoom && point.y >= rectScreenPos.y - (rectHeight / 2) * m_Camera->zoom && point.y <= rectScreenPos.y + (rectHeight / 2) * m_Camera->zoom;
}

bool Renderer::IsPointInScreenRect(const Vector2D& point, const Vector2D& rectPos, int rectWidth, int rectHeight) const
{
	return point.x >= rectPos.x - rectWidth / 2 && point.x <= rectPos.x + rectWidth / 2 && point.y >= rectPos.y - rectHeight / 2 && point.y <= rectPos.y + rectHeight / 2;
}

const Camera& Renderer::GetCamera() const
{
	return *m_Camera;
}

const Vector2D& Renderer::GetScreenSize() const
{
	return m_ScreenSize;
}