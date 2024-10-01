#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

#include "TestApplication.h"
#include "Applications/SanboxTestApp/SandboxTestApp.h"

bool InitializeSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }
    if (TTF_Init() != 0)
    {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    return true;
}

int main(int argv, char** args)
{
    InitializeSDL();

    ApplicationConfig config;
    config.Name = "Sandbox Test";
    config.ScreenSize = Vector2D(1280, 720);
    config.DrawCursor = true;

    //TestApplication* app = new TestApplication(config);
    SandboxTestApp* app = new SandboxTestApp(config);
    app->Run();
    Application::Shutdown();

    TTF_Quit();

    SDL_Quit();

    return 0;
}