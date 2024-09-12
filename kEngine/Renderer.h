#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "Camera.h"
#include "Vector2D.h"

class Renderer
{
public:
	Renderer(SDL_Window* window, const Vector2D& screenSize, Camera* camera);

	~Renderer();

	/**
	 Clears the screen.
	 */
	void Clear(SDL_Color color) const;

	/**
	 Updates the screen with any renderering performed since last call.
	 */
	void Present() const;

	/**
	 * Draws a rectangle to the screen using world coorindates.
	 *
	 * @param worldPos The world position of the rect center
	 * @param width The width of the rect
	 * @param height The height of the rect
	 * @param color The RGBA color of the rect
	 * 
	 * @note Treats the given world position as the center of the rectangle.
	 */
	void DrawRectInWorld(const Vector2D& worldPos, int width, int height, SDL_Color color) const;

	/**
	 * Draws a rectangle to the screen using screen coorindates.
	 *
	 * @param screenPos The screen position of the rect center
	 * @param width The width of the rect
	 * @param height The height of the rect
	 * @param color The RGBA color of the rect
	 *
	 * @note Treats the given screen position as the center of the rectangle.
	 */
	void DrawRectOnScreen(const Vector2D& screenPos, int width, int height, SDL_Color color) const;

	/**
	 * Draws a circle to the screen using screen coorindates.
	 *
	 * @param screenPos The screen position of the circle center
	 * @param radius The radius of the circle
	 * @param color The RGBA color of the circle
	 *
	 * @note Treats the given screen position as the center of the circle.
	 */
	void DrawCircleOnScreen(const Vector2D& screenPos, int radius, SDL_Color color) const;

	/**
	 * Draws a texture to the screen using world coorindates.
	 *
	 * @param texture The SDL_Texture
	 * @param worldPos The world position of the texture center
	 * @param width The width of the texture
	 * @param height The height of the height
	 *
	 * @note Treats the given world position as the center of the texture.
	 */
	void DrawTextureInWorld(SDL_Texture* texture, const Vector2D& worldPos, int width, int height) const;

	/**
	 * Draws a texture to the screen using screen coorindates.
	 *
	 * @param texture The SDL_Texture
	 * @param screen The screen position of the texture center
	 * @param width The width of the texture
	 * @param height The height of the height
	 *
	 * @note Treats the given screen position as the center of the texture.
	 */
	void DrawTextureOnScreen(SDL_Texture* texture, const Vector2D& screenPos, int width, int height) const;

	/**
	 * Creates an SDL_Texture from the given SDL_Surface
	 *
	 * @param surface The SDL_Surface
	 *
	 * @return The SDL_Texture
	 */
	SDL_Texture* CreateTexture(SDL_Surface* surface) const;

	// TEMPORARY
	void DrawTextOnScreen(const Vector2D& screenPos, const std::string& text, TTF_Font* font, const SDL_Color& color, bool centered) const;

	bool IsPointInWorldRect(const Vector2D& point, const Vector2D& rectPos, int rectWidth, int rectHeight) const;
	bool IsPointInScreenRect(const Vector2D& point, const Vector2D& rectPos, int rectWidth, int rectHeight) const;

	const Camera& GetCamera() const;
	const Vector2D& GetScreenSize() const;

private:
	SDL_Renderer* m_Renderer;
	Camera* m_Camera;
	Vector2D m_ScreenSize;
};

