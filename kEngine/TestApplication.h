#pragma once

#include "Application.h"
#include <vector>
#include <unordered_map>
#include "Body.h"
#include "UIElement.h"

class TestApplication : public Application
{
public:
	TestApplication(const ApplicationConfig& config);
	~TestApplication();

	void HandleEvents() override;
	void Update(double deltaTime) override;
	void Render() override;

	void AddBody(std::unique_ptr<Body> body);
	void RemoveBody(size_t index);
	void UpdateBodies(double deltaTime);
	void RenderBodies() const;

	void AddUIElement(const std::string& name, std::unique_ptr<UIElement> element);
	UIElement* GetUIElement(const std::string& name) const;
	void RemoveUIElement(const std::string& name);
	void RenderUIElements() const;

private:
	bool m_IsPaused = false;
	std::vector<std::unique_ptr<Body>> m_Bodies;
	std::unordered_map<std::string, std::unique_ptr<UIElement>> m_UIElements;
};

