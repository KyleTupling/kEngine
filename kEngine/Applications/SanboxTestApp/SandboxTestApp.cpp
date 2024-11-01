#include "SandboxTestApp.h"
#include "Utility.h"

SandboxTestApp::SandboxTestApp(const ApplicationConfig& config)
	: Application(config)
{
	testRect = new Rectangle(Vector2D(500, 300));
	testRect->SetColor({ 240, 30, 100, 255 });

	testRect2 = new Rectangle(Vector2D(650, 300));
}

SandboxTestApp::~SandboxTestApp()
{
	delete testRect;
	testRect = nullptr;

	delete testRect2;
	testRect2 = nullptr;
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
			if (event.key.keysym.sym == SDLK_SPACE)
			{
				m_IsPaused = !m_IsPaused;
			}
		}
	}
}

void SandboxTestApp::Update(double deltaTime)
{
	m_Camera.Update(deltaTime);
	if (m_IsPaused) return;

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

	if (Rectangle::CheckCollisionSAT(*testRect, *testRect2))
	{
		testRect2->SetColor({ 0, 200, 200, 255 });
	}
	else
	{
		testRect2->SetColor({ 255, 255, 255, 255 });
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
	std::array<Vector2D, 4> rectTransVertices = testRect->GetTransformedVertices();
	for (size_t i = 0; i < 4; i++)
	{
		if (rectTransVertices[i].y >= m_Config.ScreenSize.y)
		{
			Vector2D resolveForce = testRect->GetVelocity() * testRect->GetMass() * -1.2 / deltaTime;
			testRect->ApplyForce(resolveForce, rectTransVertices[i]);
		}
	}

	//if (testRect->GetPosition().y + testRect->GetHeight() / 2 >= m_Config.ScreenSize.y)
	//{
	//	// TEMPORARY: Calculate impulse force on contact with boundary
	//	Vector2D resolveForce = testRect->GetVelocity() * testRect->GetMass() * -1 / deltaTime;
	//	testRect->ApplyForce(resolveForce, testRect->GetPosition());
	//}
	testRect->Update(deltaTime);
}

void SandboxTestApp::Render()
{
	if (m_IsPaused)
	{
		m_Renderer->DrawTextOnScreen(Vector2D(m_Config.ScreenSize.x - 150, 20), "PAUSED", ResourceManager::GetInstance().LoadFont("Resources/Fonts/ARIAL.TTF", 24), { 255, 255, 255, 255 }, false);
	}

	m_Renderer->DrawTextOnScreen(Vector2D(20, 20), "SandboxTestApp", ResourceManager::GetInstance().LoadFont("Resources/Fonts/ARIAL.TTF", 24), { 255, 255, 255, 255 }, false);
	std::string angleStr = "Rect1 angle: " + Utility::ToString(testRect->GetAngle()) + "rad";
	m_Renderer->DrawTextOnScreen(Vector2D(20, 100), angleStr, ResourceManager::GetInstance().LoadFont("Resources/Fonts/ARIAL.TTF", 18), {255, 255, 255, 255}, false);

	std::string collisionStr = "Rectangles colliding: ";
	std::string collisionStateStr = Rectangle::CheckCollisionSAT(*testRect, *testRect2) ? "TRUE" : "FALSE";
	m_Renderer->DrawTextOnScreen(Vector2D(20, 140), collisionStr + collisionStateStr, ResourceManager::GetInstance().LoadFont("Resources/Fonts/ARIAL.TTF", 18), { 255, 255, 255, 255 }, false);

	m_Renderer->DrawLineInWorld(Vector2D(0, m_Config.ScreenSize.y), Vector2D(m_Config.ScreenSize.x, m_Config.ScreenSize.y), {255, 255, 255, 255});
	testRect->Draw(*m_Renderer);
	testRect2->Draw(*m_Renderer);
}