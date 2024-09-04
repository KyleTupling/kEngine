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

void Renderer::Clear() const
{
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
		screenPos.x - width / 2 * m_Camera->zoom,
		screenPos.y - height / 2 * m_Camera->zoom,
		width * m_Camera->zoom,
		height * m_Camera->zoom
	};

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(m_Renderer, &rect);
}

void Renderer::DrawRectOnScreen(const Vector2D& screenPos, int width, int height, SDL_Color color) const
{
	SDL_Rect rect = {
			screenPos.x - width / 2 * m_Camera->zoom,
			screenPos.y - height / 2 * m_Camera->zoom,
			width * m_Camera->zoom,
			height * m_Camera->zoom
	};

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(m_Renderer, &rect);
}

void Renderer::DrawTextureInWorld(SDL_Texture* texture, const Vector2D& worldPos, int width, int height) const
{
	Vector2D screenPos = m_Camera->ConvertWorldToScreen(worldPos, m_ScreenSize);

	SDL_Rect rect = {
		screenPos.x - (width / 2) * m_Camera->zoom,
		screenPos.y + (height / 2) * m_Camera->zoom,
		width * m_Camera->zoom,
		height * m_Camera->zoom
	};

	SDL_RenderCopy(m_Renderer, texture, NULL, &rect);
}

void Renderer::DrawTextureOnScreen(SDL_Texture* texture, const Vector2D& screenPos, int width, int height) const
{
	SDL_Rect rect = {
			screenPos.x - (width / 2) * m_Camera->zoom,
			screenPos.y + (height / 2) * m_Camera->zoom,
			width * m_Camera->zoom,
			height * m_Camera->zoom
	};

	SDL_RenderCopy(m_Renderer, texture, NULL, &rect);
}