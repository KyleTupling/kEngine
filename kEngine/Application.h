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

	/**
	Shuts down the ResourceManager and releases instance.
	 */
	static void Shutdown();

	/**
	Starts running the application. This will happen until m_IsRunning is false.
	 */
	void Run();

	/**
	Handles events.
	 */
	virtual void HandleEvents();

	/**
	 * Use to update logic within the application.
	 *
	 * @param deltaTime The time in seconds since the last frame.
	 */
	virtual void Update(double deltaTime) = 0;

	/**
	Use for all rendering within application.
	 */
	virtual void Render() = 0;
	
	SDL_Window& GetWindow() const { return *m_Window; }

	const Vector2D& GetScreenSize() const;

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

