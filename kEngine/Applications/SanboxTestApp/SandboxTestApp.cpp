#include "SandboxTestApp.h"
#include "Utility.h"

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
	}
}

void SandboxTestApp::Update(double deltaTime)
{
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	// Apply downwards 400N force on rectangle at mouse position
	if (keyboardState[SDL_SCANCODE_Q])
	{
		//Get mouse position
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		Vector2D mousePos(mouseX, mouseY);
		testRect->ApplyForce(Vector2D(0, 400), mousePos);
	}

	// For testing angular acceleration due to gravity
	float rectWidth = testRect->GetWidth();
	float rectHeight = testRect->GetHeight();

	// Apply gravitational force to each of rectangles vertices
	Vector2D gravityForce = Vector2D(0, 9.81) * testRect->GetMass();
	testRect->ApplyForce(gravityForce, testRect->GetPosition() - Vector2D(rectWidth / 2, rectHeight / 2));
	testRect->ApplyForce(gravityForce, testRect->GetPosition() + Vector2D(rectWidth / 2, - rectHeight / 2));
	testRect->ApplyForce(gravityForce, testRect->GetPosition() - Vector2D(rectWidth / 2, - rectHeight / 2));
	testRect->ApplyForce(gravityForce, testRect->GetPosition() + Vector2D(rectWidth / 2, rectHeight / 2));

	// Apply upwards force if rectangle falls to bottom of screen
	if (testRect->GetPosition().y + testRect->GetHeight() / 2 >= m_Config.ScreenSize.y)
	{
		testRect->ApplyForce(Vector2D(0, -10000), testRect->GetPosition());
	}
	testRect->Update(deltaTime);

	m_Camera.Update(deltaTime);
}

void SandboxTestApp::Render()
{
	m_Renderer->DrawTextOnScreen(Vector2D(20, 20), "SandboxTestApp", ResourceManager::GetInstance().LoadFont("Resources/Fonts/ARIAL.TTF", 24), { 255, 255, 255, 255 }, false);
	std::string angleStr = "Rect angle: " + Utility::ToString(testRect->GetAngle()) + "rad";
	m_Renderer->DrawTextOnScreen(Vector2D(20, 100), angleStr, ResourceManager::GetInstance().LoadFont("Resources/Fonts/ARIAL.TTF", 18), {255, 255, 255, 255}, false);

	m_Renderer->DrawLineInWorld(Vector2D(0, m_Config.ScreenSize.y), Vector2D(m_Config.ScreenSize.x, m_Config.ScreenSize.y), {255, 255, 255, 255});
	testRect->Draw(*m_Renderer);
}