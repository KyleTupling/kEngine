#pragma once

#include <SDL.h>
#include "Renderer.h"
#include "Vector2D.h"

class UIElement
{
public:
	virtual void Draw(const Renderer& renderer) = 0;
	virtual void HandleEvent(const SDL_Event& event) = 0;
	virtual ~UIElement() {}

	virtual void CheckHover(const Vector2D& mousePos, const Renderer& renderer) { };

	virtual const Vector2D& GetPosition() const { return m_Position; };
	virtual void SetPosition(const Vector2D& position) { m_Position = position; };

	bool GetIsFixedToScreen() const { return m_IsFixedToScreen; }

	virtual void SetIsFixedToScreen(bool isFixed) { m_IsFixedToScreen = isFixed; }

protected:
	Vector2D m_Position;
	bool m_IsFixedToScreen = true; // Whether UIElement position is fixed on screen
};

