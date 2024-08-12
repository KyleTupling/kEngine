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

	bool GetIsFixedToScreen() const { return m_isFixedToScreen; }

	void SetIsFixedToScreen(bool isFixed) { m_isFixedToScreen = isFixed; }

protected:
	bool m_isFixedToScreen = true;
};

