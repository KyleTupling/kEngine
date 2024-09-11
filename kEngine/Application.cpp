#include "Application.h"
#include "Utility.h"

Application* Application::s_Instance = nullptr;

Application::Application(const ApplicationConfig& config)
	: m_Config(config)
{
	if (s_Instance)
	{
		throw std::runtime_error("Application already exists.");
	}
	s_Instance = this;

	m_Camera = Camera(Vector2D(m_Config.ScreenSize.x / 2, m_Config.ScreenSize.y / 2), 1.0f);

	// TODO: Set window title to ApplicationConfig Name
	m_Window = SDL_CreateWindow(config.Name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, static_cast<int>(m_Config.ScreenSize.x), static_cast<int>(m_Config.ScreenSize.y), SDL_WINDOW_OPENGL);
	if (!m_Window)
	{
		throw std::runtime_error("Failed to create SDL Window.");
	}

	m_Renderer = std::make_unique<Renderer>(m_Window, m_Config.ScreenSize, &m_Camera);

	SDL_ShowCursor(config.DrawCursor ? SDL_ENABLE : SDL_DISABLE);
}

Application::~Application()
{
	SDL_DestroyWindow(m_Window);
}

void Application::Shutdown()
{
	ResourceManager::Shutdown();
	delete s_Instance;
	s_Instance = nullptr;
}

void Application::Run()
{
	// Initialise variables related to deltaTime
	Uint64 current = SDL_GetPerformanceCounter();
	Uint64 last = current;
	double deltaTime = 0;

	while (m_IsRunning)
	{
		current = SDL_GetPerformanceCounter();
		deltaTime = static_cast<double>((current - last) / (double)SDL_GetPerformanceFrequency());
		last = current;

		// Calculate current FPS and display on window title
	    double currentFPS = 1 / deltaTime;
	    std::string fpsStr = Utility::ToString(round(currentFPS));
	    std::string titleStr = m_Config.Name + " | FPS: " + fpsStr;
	    SDL_SetWindowTitle(m_Window, titleStr.c_str());

		HandleEvents();
		Update(deltaTime);
		m_Renderer->Clear({ 10, 10, 10, 255 });
		Render();
		m_Renderer->Present();
	}
}

void Application::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			m_IsRunning = false;
		}
	}
}

const Vector2D& Application::GetScreenSize() const
{
	int screenX, screenY;
	SDL_GetWindowSize(m_Window, &screenX, &screenY);
	return Vector2D(screenX, screenY);
}