#pragma once

#include <SDL.h>

class UIElement
{
public:
	virtual void Draw(SDL_Renderer* renderer) = 0;
	virtual void HandleEvent(const SDL_Event& event) = 0;
	virtual ~UIElement() {}
};

