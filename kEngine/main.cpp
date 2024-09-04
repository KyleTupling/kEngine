#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <memory>

#include "Renderer.h"
#include "Vector2D.h"
#include "Body.h"
#include "Camera.h"
#include "Text.h"
#include "UIButton.h"
#include "UIWindow.h"

Vector2D screenSize(1280, 720);

bool isRunning = true; // Keep track of engine running state (main loop)
bool isPaused = true;

Vector2D cursorPos(0, 0);
bool isMouseDown = false;

SDL_Color colorWhite = { 255, 255, 255, 255 };
SDL_Color colorBlack = { 0, 0, 0, 255 };
SDL_Color colorGrey = { 100, 100, 100, 255 };

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
    SDL_ShowCursor(SDL_DISABLE);
    return true;
}

// Converts given variable to a std::string
template <typename T>
std::string to_string(T x)
{
    std::ostringstream ss;
    ss << x;
    return ss.str();
}

void UpdateCamera(Camera& camera, const Vector2D& screenSize)
{
    // Handle translation
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    if (keyboardState[SDL_SCANCODE_W])
    {
        camera.position.y -= camera.moveSpeed / camera.zoom;
    }
    if (keyboardState[SDL_SCANCODE_S])
    {
        camera.position.y += camera.moveSpeed / camera.zoom;
    }
    if (keyboardState[SDL_SCANCODE_A])
    {
        camera.position.x -= camera.moveSpeed / camera.zoom;
    }
    if (keyboardState[SDL_SCANCODE_D])
    {
        camera.position.x += camera.moveSpeed / camera.zoom;
    }

    // Reset position and zoom
    if (keyboardState[SDL_SCANCODE_X])
    {
        camera.Reset(screenSize);
    }
}

int main(int argv, char** args)
{
    // Initialise variables related to deltaTime
    Uint64 current = SDL_GetPerformanceCounter();
    Uint64 last = current;
    double deltaTime = 0;

    Body body1(Vector2D(200, 50), Vector2D(350, 0));
    body1.SetColor({ 0, 0, 255, 0 });
    body1.SetShouldDrawPosHistory(true);

    Body body2(Vector2D(500, 300), Vector2D(0, 0));
    body2.SetRadius(15);
    body2.SetColor({ 230, 180, 30, 255 });
    body2.SetMass(1e18);

    // Initialise SDL
    if (!InitializeSDL())
    {
        return 1;
    }

    // TODO: Create Engine class to manage SDL window, renderer etc.
    SDL_Window* window = SDL_CreateWindow("kEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, static_cast<int>(screenSize.x), static_cast<int>(screenSize.y), SDL_WINDOW_OPENGL);
    //SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Camera camera(Vector2D(screenSize.x / 2, screenSize.y / 2), 1.0f);
    Renderer mainRenderer(window, screenSize, &camera);

    // Initialise fonts
    TTF_Font* pausedFont = TTF_OpenFont("C:/Users/PC/Desktop/cpp/ARIAL.TTF", 20);
    TTF_Font* bodyFont = TTF_OpenFont("C:/Users/PC/Desktop/cpp/ARIAL.TTF", 18);

    auto pausedLabel = std::make_unique<UILabel>(mainRenderer, Vector2D(1280 - 100, 50), "PAUSED", pausedFont, colorWhite, colorBlack);
    pausedLabel->SetDrawBackground(true);
    pausedLabel->SetTextColor({ 255, 0, 0, 255 });
    //auto bodyLabel = std::make_unique<UILabel>(renderer, static_cast<int>(body1.position.x), static_cast<int>(body1.position.y + body1.radius) + 10, "Planet", bodyFont, colorBlack, colorWhite);
    //bodyLabel->SetIsFixedToScreen(false);

    //auto mainWindow = std::make_unique<UIWindow>(renderer, body1.position.x + 50, body1.position.y - 20, 400, 200, "Planet", bodyFont);
    //mainWindow->SetIsFixedToScreen(false);
    //auto windowButton = std::make_unique<UIButton>(renderer, mainWindow->GetPosX(), mainWindow->GetPosY() + mainWindow->GetHeight() - 40, mainWindow->GetWidth(), 40, "Change Body Color", bodyFont, colorGrey);
    //windowButton->SetOnClick([&body1]()
    //    {
    //        body1.color.r = 100;
    //    });
    //mainWindow->AddUIElement(std::move(windowButton));
    //auto windowLabel = std::make_unique<UILabel>(renderer, 180, 550, "(" + to_string(std::round(body1.position.x)) + ", " + to_string(std::round(body1.position.y)) + ")", bodyFont, colorWhite);
    //// Temporary: Store raw pointer to label to access within lambda function
    //auto* labelPtr = windowLabel.get();
    //windowLabel->SetTextUpdater([labelPtr, &body1]()
    //    {
    //        labelPtr->SetText("Position: (" + to_string(std::round(body1.position.x)) + ", " + to_string(std::round(body1.position.y)) + ")");
    //    });
    //mainWindow->AddUIElement(std::move(windowLabel));

    while (isRunning)
    {
        // Calculate deltaTime each frame
        current = SDL_GetPerformanceCounter();
        deltaTime = static_cast<double>((current - last) / (double)SDL_GetPerformanceFrequency());
        last = current;

        // Calculate current FPS and display on window title
        double currentFPS = 1 / deltaTime;
        std::string fpsStr = to_string(round(currentFPS));
        std::string titleStr = "kEngine | FPS: " + fpsStr;
        SDL_SetWindowTitle(window, titleStr.c_str());

        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
                break;
            }

            // Pause simulation if window is dragged (prevents step jumps)
            if (event.window.event == SDL_WINDOWEVENT_MOVED)
            {
                isPaused = true;
            }

            //mainWindow->HandleEvent(event);

            if (event.type == SDL_MOUSEMOTION)
            {
                // Store current mouse position on mouse move
                cursorPos.Set(event.motion.x, event.motion.y);
                //mainWindow->CheckHover(cursorPos, camera, screenSize);
                body1.CheckHover(cursorPos, camera, screenSize);
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                isMouseDown = true;
            }
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                isMouseDown = false;
            }

            if (event.type == SDL_KEYDOWN)
            {
                // Pause
                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    isPaused = !isPaused;
                }
                
                if (event.key.keysym.sym == SDLK_m)
                {
                    //if (!mainWindow->GetIsDisplayed()) mainWindow->SetIsDisplayed(true);
                }

                if (event.key.keysym.sym == SDLK_n)
                {
                    //mainWindow->SetIsFixedToScreen(!mainWindow->GetIsFixedToScreen());
                }
            }

            if (event.type == SDL_MOUSEWHEEL)
            {
                //if (event.wheel.y > 0) // Scroll up
                //{
                //    camera.Zoom(cursorPos, screenSize, true);
                //}
                //else if (event.wheel.y < 0) { // Scroll down
                //    camera.Zoom(cursorPos, screenSize, false);
                //}
                if (event.wheel.y > 0 && camera.targetZoom < camera.maxZoom) camera.targetZoom += event.wheel.y * 0.5;
                else if (event.wheel.y < 0 && camera.targetZoom > camera.minZoom) camera.targetZoom += event.wheel.y * 0.5;
            }
        }

        UpdateCamera(camera, screenSize);
        camera.Update(deltaTime);

        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        //SDL_RenderClear(renderer); // Clear render window each frame before populating again

        mainRenderer.Clear({ 255, 255, 255, 255 });

        if (!isPaused) // Only update physics when simulation isn't paused
        {
            body2.AttractBody(body1);

            body1.Update(deltaTime);
            body2.Update(deltaTime);

            // Keep bodyLabel under body render
            //bodyLabel->SetPosition(static_cast<int>(body1.position.x), static_cast<int>(body1.position.y + body1.radius) + 10);

            // Testing sticking window to body
           // mainWindow->SetPosX(body1.position.x + 50);
            //mainWindow->SetPosY(body1.position.y - 20);

            //if (isMouseDown) // If mouse is clicked, apply force to body1 directly proportional to distance between the body and the mouse
            //{
            //    double cursorDist = (cursorPos - body1.position).GetMagnitude();
            //    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            //    DrawCircle(renderer, cursorPos.x, cursorPos.y, cursorDist * 0.2); // Display circle around cursor with a radius proportional to the distance between the cursor and the body
            //    body1.ApplyForce((cursorPos - body1.position) * 25);
            //}
        }

        if (isPaused)
        {
            pausedLabel->Draw(mainRenderer);
        }

        body2.Draw(mainRenderer);
        body1.Draw(mainRenderer);

        //bodyText.Draw(camera, screenSize);
        //bodyLabel->Draw(renderer, camera, screenSize);

        // Draw UI on top of scene
        //mainWindow->Draw(renderer, camera, screenSize);

        // Draw cursor
        mainRenderer.DrawCircleOnScreen(cursorPos, 6, { 255, 255, 255, 255 });
        mainRenderer.DrawCircleOnScreen(cursorPos, 5, { 0, 0, 0, 255 });

        //SDL_RenderPresent(renderer);
        mainRenderer.Present();
    }

    //SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_CloseFont(pausedFont);
    TTF_CloseFont(bodyFont);

    TTF_Quit();

    SDL_Quit();

    return 0;
}