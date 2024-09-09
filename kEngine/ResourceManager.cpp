#include "ResourceManager.h"

TTF_Font* ResourceManager::LoadFont(const std::string& path, int fontSize)
{
	// Create identifier from path and size
	std::string key = path + "_" + std::to_string(fontSize);

	// Check if font is already loaded
	auto iter = m_Fonts.find(key);
	if (iter != m_Fonts.end())
	{
		return iter->second.get();
	}

	// Load font
	TTF_Font* font = TTF_OpenFont(path.c_str(), fontSize);
	if (!font)
	{
		throw std::runtime_error("Failed to load font from: " + path + ". Error: " + std::string(TTF_GetError()));
	}

	m_Fonts[key] = std::unique_ptr<TTF_Font, DeleteFont>(font);

	return font;
}