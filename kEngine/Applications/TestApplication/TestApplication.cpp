#include "TestApplication.h"
#include "UILabel.h"
#include "UIWindow.h"
#include "Utility.h"
#include "MathFunctions.h"

TestApplication::TestApplication(const ApplicationConfig& config)
	: Application(config)
{
	// Create star body
	auto starBody = std::make_unique<Body>(Vector2D(m_Config.ScreenSize.x / 2, m_Config.ScreenSize.y / 2), Vector2D(0, 0));
	starBody->SetMass(5e17);
	starBody->SetRadius(15);
	starBody->SetColor({ 230, 180, 30, 255 });
	starBody->SetHoveredColor({ 250, 210, 50, 255 });
	starBody->SetName("Star");
	m_Bodies.push_back(std::move(starBody));

	// Create planet body
	auto planetBody = std::make_unique<Body>(Vector2D(640, 150), Vector2D(400, 0));
	planetBody->SetMass(10);
	planetBody->SetColor({ 60, 125, 210, 255 });
	planetBody->SetHoveredColor({ 60, 60, 240, 255 });
	planetBody->SetShouldDrawPosHistory(true);
	planetBody->SetName("Planet");
	m_Bodies.push_back(std::move(planetBody));

	// Load font
	TTF_Font* pausedFont = GetResourceManager().LoadFont("Resources/Fonts/ARIAL.TTF", 20);

	// TEMPORARY: Cache colors
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color offBlack = { 20, 20, 20, 255 };

	// Add paused label
	AddUIElement("pausedLabel", std::make_unique<UILabel>(*m_Renderer, Vector2D(1280 - 100, 50), "PAUSED", pausedFont, white));
	GetUIElement("pausedLabel")->SetIsVisible(m_IsPaused);

	// Create window attached to planet body
	AddUIElement("bodyWindow", std::make_unique<UIWindow>(*m_Renderer, Vector2D(100, 100), 200, 100, "Body", pausedFont));
	// TODO: To make this safer, implement enum class UIELementType into UIElement?
	UIWindow* bodyWindow = dynamic_cast<UIWindow*>(GetUIElement("bodyWindow"));
	bodyWindow->SetBackgroundColor({ 40, 40, 40, 100 });
	bodyWindow->SetIsFixedToScreen(false);

	// Add button to toggle whether window is fixed to screen
	auto pinButton = std::make_unique<UIButton>(*m_Renderer, bodyWindow->GetPosition() - Vector2D(bodyWindow->GetWidth() / 2 - 15, bodyWindow->GetHeight() / 2 - 15), 30, 30, "P", pausedFont, offBlack);
	pinButton->SetOnClick([=] {
		UIWindow* window = dynamic_cast<UIWindow*>(GetUIElement("bodyWindow"));
		if (window)
		{
			window->SetIsFixedToScreen(!window->GetIsFixedToScreen());
		}
	});
	bodyWindow->AddUIElement(std::move(pinButton));

	// Create details window to display information about bodies
	TTF_Font* detailsFont = GetResourceManager().LoadFont("Resources/Fonts/ARIAL.TTF", 18);
	AddUIElement("sideWindow", std::make_unique<UIWindow>(*m_Renderer, Vector2D(100, config.ScreenSize.y / 2), 200, config.ScreenSize.y, "Details", pausedFont));
	for (size_t i = 0; i < m_Bodies.size(); i++)
	{
		auto bodyDetailsWindow = std::make_unique<UIWindow>(*m_Renderer, Vector2D(100, 90 + 110 * i), 200, 100, "", detailsFont);
		bodyDetailsWindow->SetDrawTitleBar(false);
		bodyDetailsWindow->SetBackgroundColor({ 20, 20, 20, 100 });

		auto bodyIconButton = std::make_unique<UIButton>(*m_Renderer, Vector2D(30, 90 + 110 * i - 20), 40, 40, "", detailsFont, m_Bodies[i]->GetColor());
		// Set button hover color procedurally
		SDL_Color hoverColor = { 0, 0, 0, 255 };
		float hoverColorMultiplier = 1.2f;
		hoverColor.r = MathFunctions::clamp((int)std::round(m_Bodies[i]->GetColor().r * hoverColorMultiplier), 0, 255);
		hoverColor.g = MathFunctions::clamp((int)std::round(m_Bodies[i]->GetColor().g * hoverColorMultiplier), 0, 255);
		hoverColor.b = MathFunctions::clamp((int)std::round(m_Bodies[i]->GetColor().b * hoverColorMultiplier), 0, 255);
		bodyIconButton->SetHoveredColor(hoverColor);
		bodyDetailsWindow->AddUIElement(std::move(bodyIconButton));

		auto bodyNameLabel = std::make_unique<UILabel>(*m_Renderer, Vector2D(100, 90 + 110 * i - 30), m_Bodies[i]->GetName(), detailsFont, white);
		bodyDetailsWindow->AddUIElement(std::move(bodyNameLabel));

		auto bodyMassLabel = std::make_unique<UILabel>(*m_Renderer, Vector2D(100, 90 + 110 * i - 10), Utility::ToString(m_Bodies[i]->GetMass()) + "kg", detailsFont, white);
		bodyDetailsWindow->AddUIElement(std::move(bodyMassLabel));

		auto gotoButton = std::make_unique<UIButton>(*m_Renderer, Vector2D(160, 90 + 110 * i + 30), 60, 30, "Goto", detailsFont, offBlack);
		gotoButton->SetColor({ 10, 10, 10, 255 });
		gotoButton->SetHoveredColor({ 40, 40, 40, 255 });
		gotoButton->SetOnClick([=] {
			m_TrackedBodyIndex = i;
		});

		bodyDetailsWindow->AddUIElement(std::move(gotoButton));

		dynamic_cast<UIWindow*>(GetUIElement("sideWindow"))->AddUIElement(std::move(bodyDetailsWindow));
	}
}

TestApplication::~TestApplication()
{
	
}

// TODO: Dispatch events from Application to child
void TestApplication::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			m_IsRunning = false;
		}

		if (event.type == SDL_KEYDOWN)
		{
			m_TrackedBodyIndex = -1;

			// Pause - still render but don't update
			if (event.key.keysym.sym == SDLK_SPACE)
			{
				m_IsPaused = !m_IsPaused;
				GetUIElement("pausedLabel")->SetIsVisible(m_IsPaused);
			}

			if (event.key.keysym.sym == SDLK_e)
			{
				GetUIElement("sideWindow")->SetIsVisible(true);
			}

			// TEMPORARY: Move position of planet body
			if (event.key.keysym.sym == SDLK_q)
			{
				// Get mouse position
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				Vector2D worldMousePos = m_Renderer->GetCamera().ConvertScreenToWorld(Vector2D(mouseX, mouseY), m_Renderer->GetScreenSize());
				m_Bodies[1]->SetPosition(worldMousePos);
			}

			// TEMPORARY: Toggle position projection rendering
			if (event.key.keysym.sym == SDLK_p)
			{
				m_DrawPosProjection = !m_DrawPosProjection;
			}
		}

		if (event.type == SDL_MOUSEMOTION)
		{
			// Get mouse position
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);

			// Check hover state of all visible UIElements
			for (const auto& pair : m_UIElements)
			{
				if (pair.second->GetIsVisible())
				{
					pair.second->CheckHover(Vector2D(mouseX, mouseY), *m_Renderer);
				}
			}

			// Check hover state of all bodies
			for (auto& body : m_Bodies)
			{
				body->CheckHover(Vector2D(mouseX, mouseY), *m_Renderer);
			}
		}

		if (event.type == SDL_MOUSEWHEEL)
		{
			// Zoom camera
			if (event.wheel.y > 0 && m_Camera.targetZoom < m_Camera.maxZoom) m_Camera.targetZoom += event.wheel.y * 0.5;
			else if (event.wheel.y < 0 && m_Camera.targetZoom > m_Camera.minZoom) m_Camera.targetZoom += event.wheel.y * 0.5;
		}

		// Handle event for all visible UIElements
		for (const auto& pair : m_UIElements)
		{
			if (pair.second->GetIsVisible())
			{
				pair.second->HandleEvent(event);
			}
		}
	}
}

void TestApplication::Update(double deltaTime)
{
	// Handle camera translation
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	if (keyboardState[SDL_SCANCODE_W])
	{
		m_Camera.position.y -= m_Camera.moveSpeed / m_Camera.zoom;
	}
	if (keyboardState[SDL_SCANCODE_S])
	{
		m_Camera.position.y += m_Camera.moveSpeed / m_Camera.zoom;
	}
	if (keyboardState[SDL_SCANCODE_A])
	{
		m_Camera.position.x -= m_Camera.moveSpeed / m_Camera.zoom;
	}
	if (keyboardState[SDL_SCANCODE_D])
	{
		m_Camera.position.x += m_Camera.moveSpeed / m_Camera.zoom;
	}

	// Reset camera position and zoom
	if (keyboardState[SDL_SCANCODE_X])
	{
		m_Camera.Reset(m_Config.ScreenSize);
	}

	// Call camera update for target zoom lerp
	m_Camera.Update(deltaTime);

	// Update all bodies
	UpdateBodies(deltaTime);

	// Update bodyWindow position to stick to planetBody (if bodyWindow isn't fixed to screen)
	UIWindow* bodyWindow = dynamic_cast<UIWindow*>(GetUIElement("bodyWindow"));
	if (!bodyWindow->GetIsFixedToScreen())
	{
		bodyWindow->SetPosition(m_Bodies[1]->GetPosition() + Vector2D(120, 0));
	}
}

void TestApplication::Render()
{
	RenderBodies();

	RenderUIElements();

	if (!GetUIElement("sideWindow")->GetIsVisible())
	{
		m_Renderer->DrawTextOnScreen(Vector2D(m_Config.ScreenSize.x / 2, m_Config.ScreenSize.y - 20), "Press 'E' to display details panel", GetResourceManager().LoadFont("Resources/Fonts/ARIAL.TTF", 20), { 255, 255, 255, 255 }, true);
	}
}

void TestApplication::AddBody(std::unique_ptr<Body> body)
{
	m_Bodies.push_back(std::move(body));
}

void TestApplication::UpdateBodies(double deltaTime)
{
	if (m_IsPaused) return;

	for (auto& body : m_Bodies)
	{
		for (auto& otherBody : m_Bodies)
		{
			if (body == otherBody)
			{
				continue;
			}
			body->AttractBody(*otherBody);
		}
	}

	for (auto& body : m_Bodies)
	{
		body->Update(deltaTime);
	}

	if (m_TrackedBodyIndex >= 0)
	{
		if (m_Bodies[m_TrackedBodyIndex])
		{
			m_Camera.position = m_Bodies[m_TrackedBodyIndex]->GetPosition();
		}
		
	}
}

void TestApplication::RenderBodies() const
{
	if (m_DrawPosProjection)
	{
		std::vector<Vector2D> projectedPositions = m_Bodies[1]->ProjectPosition(1000, m_Bodies); // Project position test
		for (auto& pos : projectedPositions)
		{
			m_Renderer->DrawRectInWorld(pos, 2, 2, { 255, 255, 255, 255 });
		}
	}
	
	for (const auto& body : m_Bodies)
	{
		body->Draw(*m_Renderer);
	}
}

void TestApplication::RemoveBody(size_t index)
{
	if (index < m_Bodies.size())
	{
		m_Bodies.erase(m_Bodies.begin() + index);
	}
}

void TestApplication::AddUIElement(const std::string& name, std::unique_ptr<UIElement> element)
{
	m_UIElements[name] = std::move(element);
}

UIElement* TestApplication::GetUIElement(const std::string& name) const
{
	auto iter = m_UIElements.find(name);
	if (iter != m_UIElements.end())
	{
		return iter->second.get();
	}
	return nullptr;
}

void TestApplication::RemoveUIElement(const std::string& name)
{
	auto iter = m_UIElements.find(name);
	if (iter != m_UIElements.end())
	{
		m_UIElements.erase(iter);
	}
}

void TestApplication::RenderUIElements() const
{
	for (const auto& pair : m_UIElements)
	{
		if (pair.second->GetIsVisible())
		{
			pair.second->Draw(*m_Renderer);
		}
	}
}