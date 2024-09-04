#pragma once

#include <SDL.h>
#include "Camera.h"
#include "Vector2D.h"

class UIElement
{
public:
	virtual void Draw(SDL_Renderer* renderer, const Camera& camera, const Vector2D& screenSize) = 0;
	virtual void HandleEvent(const SDL_Event& event) = 0;
	virtual ~UIElement() {}

	virtual void CheckHover(const Vector2D& mousePos, const Camera& camera, const Vector2D& screenSize) { };

	virtual int GetPosX() const { return m_posX; }
	virtual int GetPosY() const { return m_posY; }

	bool GetIsFixedToScreen() const { return m_isFixedToScreen; }

	virtual void SetPosX(int x) { m_posX = x; }
	virtual void SetPosY(int y) { m_posY = y; }
	virtual void SetIsFixedToScreen(bool isFixed) { m_isFixedToScreen = isFixed; }

protected:
	int m_posX;
	int m_posY;
	bool m_isFixedToScreen = true; // Whether UIElement position is fixed on screen
};

