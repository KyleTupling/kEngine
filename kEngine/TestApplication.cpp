#include "TestApplication.h"
#include "UILabel.h"
#include "UIWindow.h"

TestApplication::TestApplication(const ApplicationConfig& config)
	: Application(config)
{
	auto starBody = std::make_unique<Body>(Vector2D(m_Config.ScreenSize.x / 2, m_Config.ScreenSize.y / 2), Vector2D(0, 0));
	starBody->SetMass(1e18);
	starBody->SetRadius(15);
	starBody->SetColor({ 230, 180, 30, 255 });
	starBody->SetHoveredColor({ 250, 210, 50, 255 });
	m_Bodies.push_back(std::move(starBody));

	auto planetBody = std::make_unique<Body>(Vector2D(400, 200), Vector2D(400, 0));
	planetBody->SetMass(10);
	planetBody->SetColor({ 30, 30, 210, 255 });
	planetBody->SetHoveredColor({ 60, 60, 240, 255 });
	planetBody->SetShouldDrawPosHistory(true);
	m_Bodies.push_back(std::move(planetBody));

	TTF_Font* pausedFont = GetResourceManager().LoadFont("Resources/Fonts/ARIAL.TTF", 20);
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color offBlack = { 20, 20, 20, 255 };
	AddUIElement("pausedLabel", std::make_unique<UILabel>(*m_Renderer, Vector2D(1280 - 100, 50), "PAUSED", pausedFont, white));
	GetUIElement("pausedLabel")->SetIsVisible(false);

	AddUIElement("bodyWindow", std::make_unique<UIWindow>(*m_Renderer, Vector2D(100, 100), 200, 100, "Body", pausedFont));
	// TODO: To make this safer, implement enum class UIELementType into UIElement?
	UIWindow* bodyWindow = dynamic_cast<UIWindow*>(GetUIElement("bodyWindow"));
	bodyWindow->SetBackgroundColor({ 40, 40, 40, 100 });
	bodyWindow->SetIsFixedToScreen(false);

	auto pinButton = std::make_unique<UIButton>(*m_Renderer, bodyWindow->GetPosition() - Vector2D(bodyWindow->GetWidth() / 2 - 15, bodyWindow->GetHeight() / 2 - 15), 30, 30, "P", pausedFont, offBlack);
	// Add button to toggle whether window is fixed to screen
	pinButton->SetOnClick([this]()
		{
			UIWindow* window = dynamic_cast<UIWindow*>(GetUIElement("bodyWindow"));
			if (window)
			{
				window->SetIsFixedToScreen(!window->GetIsFixedToScreen());
			}
		});
	bodyWindow->AddUIElement(std::move(pinButton));
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
			if (event.key.keysym.sym == SDLK_SPACE)
			{
				m_IsPaused = !m_IsPaused;
				GetUIElement("pausedLabel")->SetIsVisible(m_IsPaused);
			}
		}

		if (event.type == SDL_MOUSEMOTION)
		{
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

			for (auto& body : m_Bodies)
			{
				body->CheckHover(Vector2D(mouseX, mouseY), *m_Renderer);
			}
		}

		if (event.type == SDL_MOUSEWHEEL)
		{
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
	// Reset position and zoom
	if (keyboardState[SDL_SCANCODE_X])
	{
		m_Camera.Reset(m_Config.ScreenSize);
	}

	m_Camera.Update(deltaTime);
	UpdateBodies(deltaTime);

	UIWindow* bodyWindow = dynamic_cast<UIWindow*>(GetUIElement("bodyWindow"));
	if (!bodyWindow->GetIsFixedToScreen())
	{
		bodyWindow->SetPosition(m_Bodies[1]->GetPosition() + Vector2D(120, 0));
	}
	
}

void TestApplication::Render()
{
	RenderUIElements();
	RenderBodies();
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
}

void TestApplication::RenderBodies() const
{
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