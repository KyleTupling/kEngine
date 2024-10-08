#pragma once

#include "Application.h"
#include "Rectangle.h"

class SandboxTestApp : public Application
{
public:
	SandboxTestApp(const ApplicationConfig& config);
	~SandboxTestApp();

	void HandleEvents() override;
	void Update(double deltaTime) override;
	void Render() override;
private:
	bool m_IsPaused = false;

	Rectangle* testRect = nullptr;
	Rectangle* testRect2 = nullptr;
};

