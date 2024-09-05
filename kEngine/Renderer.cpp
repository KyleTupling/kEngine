#include "Renderer.h"

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

void Renderer::DrawRectInWorld(const Vector2D& worldPos, int width, int height, SDL_Color color) const
{
	Vector2D screenPos = m_Camera->ConvertWorldToScreen(worldPos, m_ScreenSize);

	SDL_Rect rect = {
		static_cast<int>((screenPos.x - width / 2) * m_Camera->zoom),
		static_cast<int>((screenPos.y - height / 2) * m_Camera->zoom),
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
		static_cast<int>((screenPos.x - width / 2) * m_Camera->zoom),
		static_cast<int>((screenPos.y - height / 2) * m_Camera->zoom),
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

bool Renderer::IsPointInWorldRect(const Vector2D& point, const Vector2D& rectPos, int rectWidth, int rectHeight) const
{
	Vector2D rectScreenPos = m_Camera->ConvertWorldToScreen(rectPos, m_ScreenSize);
	return point.x >= (rectScreenPos.x - rectWidth / 2) * m_Camera->zoom && point.x <= (rectScreenPos.x + rectWidth / 2) * m_Camera->zoom && point.y >= (rectScreenPos.y - rectHeight / 2) * m_Camera->zoom && point.y <= (rectScreenPos.y + rectHeight / 2) * m_Camera->zoom;
}

bool Renderer::IsPointInScreenRect(const Vector2D& point, const Vector2D& rectPos, int rectWidth, int rectHeight) const
{
	return point.x >= rectPos.x - rectWidth / 2 && point.x <= rectPos.x + rectWidth / 2 && point.y >= rectPos.y - rectHeight / 2 && point.y <= rectPos.y + rectHeight / 2;
}