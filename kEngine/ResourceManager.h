#pragma once

#include <SDL_ttf.h>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include <memory>

class ResourceManager
{
public:
	static ResourceManager& GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	TTF_Font* LoadFont(const std::string& path, int fontSize);

	static void Shutdown()
	{
		GetInstance().m_Fonts.clear();
	}

	// Delete copy constructor and assigment operator
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
private:
	ResourceManager() = default;

	// Use struct with operator() override to create functor
	struct DeleteFont
	{
		void operator()(TTF_Font* font) const
		{
			TTF_CloseFont(font);
		}
	};

	std::unordered_map<std::string, std::unique_ptr<TTF_Font, DeleteFont>> m_Fonts;
};

