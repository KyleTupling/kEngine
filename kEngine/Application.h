#pragma once

#include "Camera.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Vector2D.h"
#include <SDL.h>

struct ApplicationConfig
{
	std::string Name = "kEngine Application";
	Vector2D ScreenSize = Vector2D(1280, 720);
	bool DrawCursor = true;
};

class Application
{
public:
	Application(const ApplicationConfig& config);
	virtual ~Application();

	static void Shutdown();

	//virtual void Init();
	void Run();
	virtual void HandleEvents();
	virtual void Update(double deltaTime) = 0;
	virtual void Render() = 0;
	
	SDL_Window& GetWindow() const { return *m_Window; }

	ResourceManager& GetResourceManager() { return m_ResourceManager; }

	static Application& Get() { return *s_Instance; }

	const ApplicationConfig& GetConfig() const { return m_Config; }
protected:
	ApplicationConfig m_Config;
	Camera m_Camera;
	std::unique_ptr<Renderer> m_Renderer;
	bool m_IsRunning = true;
private:
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	static Application* s_Instance;
	SDL_Window* m_Window;
	ResourceManager& m_ResourceManager = ResourceManager::GetInstance();
};

