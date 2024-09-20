#include "SandboxTestApp.h"

SandboxTestApp::SandboxTestApp(const ApplicationConfig& config)
	: Application(config)
{
	testRect = new Rectangle(Vector2D(500, 300));
	testRect->SetColor({ 240, 30, 100, 255 });
}

SandboxTestApp::~SandboxTestApp()
{
	delete testRect;
	testRect = nullptr;
}

void SandboxTestApp::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			m_IsRunning = false;
		}

		if (event.type == SDL_MOUSEWHEEL)
		{
			// Zoom camera
			if (event.wheel.y > 0 && m_Camera.targetZoom < m_Camera.maxZoom) m_Camera.targetZoom += event.wheel.y * 0.5;
			else if (event.wheel.y < 0 && m_Camera.targetZoom > m_Camera.minZoom) m_Camera.targetZoom += event.wheel.y * 0.5;
		}

		if (event.type == SDL_KEYDOWN)
		{
			// Apply downwards force to rectangle at mouse position
			if (event.key.keysym.sym == SDLK_q)
			{
				// Get mouse position
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				testRect->ApplyForce(Vector2D(0, 250), Vector2D(mouseX, mouseY));
			}
		}
	}
}

void SandboxTestApp::Update(double deltaTime)
{
	testRect->Update(deltaTime);

	m_Camera.Update(deltaTime);
}

void SandboxTestApp::Render()
{
	testRect->Draw(*m_Renderer);
}