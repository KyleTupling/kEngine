#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>

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

// Converts given variable to a std::string
template <typename T>
std::string to_string(T x)
{
    std::ostringstream ss;
    ss << x;
    return ss.str();
}

// Temporary procedure from SE
void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
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
        camera.position.Set(screenSize.x / 2, screenSize.y / 2);
        camera.zoom = 1.0f;
        camera.targetZoom = 1.0f;
    }
}

int main(int argv, char** args)
{
    // Initialise variables related to deltaTime
    Uint64 current = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    double deltaTime = 0;

    Body body1(Vector2D(200, 50), Vector2D(350, 0));
    body1.color.b = 255;
    body1.drawPrevious = true;

    Body body2(Vector2D(500, 300), Vector2D(0, 0));
    body2.radius = 15;
    body2.color.r = 230;
    body2.color.g = 180;
    body2.color.b = 30;
    body2.mass = 1e18;

    // Initialise SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();
    SDL_ShowCursor(SDL_DISABLE);

    // TODO: Create Engine class to manage SDL window, renderer etc.
    SDL_Window* window = SDL_CreateWindow("kEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenSize.x, screenSize.y, SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Camera camera(Vector2D(screenSize.x / 2, screenSize.y / 2), 1.0f);

    // Initialise fonts
    TTF_Font* pausedFont = TTF_OpenFont("C:/Users/PC/Desktop/cpp/ARIAL.TTF", 20);
    TTF_Font* bodyFont = TTF_OpenFont("C:/Users/PC/Desktop/cpp/ARIAL.TTF", 18);

    // Initialise Text
    // Soon to be phased out and replaced by UILabel
    Text pausedText(renderer, "PAUSED", pausedFont, 1280 - 100, 50, { 255, 255, 255, 255 });
    pausedText.setIsFixed(true);
    SDL_Rect pausedRect{ pausedText.getPosX() - 50, pausedText.getPosY() - 20, 100, 40};

    Text bodyText(renderer, "Planet", bodyFont, static_cast<int>(body1.position.x), static_cast<int>(body1.position.y + body1.radius) + 10);
    std::string bodyInfoStr = to_string(body1.velocity.x);

    auto mainWindow = std::make_unique<UIWindow>(renderer, 100, 500, 400, 200, "Planet", bodyFont);
    mainWindow->AddUIElement(std::make_unique<UIButton>(105, 570, 50, 20, "Test", bodyFont, colorGrey));

    while (isRunning)
    {
        // Calculate deltaTime each frame
        last = current;
        current = SDL_GetPerformanceCounter();
        deltaTime = static_cast<double>((current - last) / (double)SDL_GetPerformanceFrequency());

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

            mainWindow->HandleEvent(event);

            if (event.type == SDL_MOUSEMOTION)
            {
                // Store current mouse position on mouse move
                cursorPos.Set(event.motion.x, event.motion.y);

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
                    if (!mainWindow->GetIsDisplayed()) mainWindow->SetIsDisplayed(true);
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

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer); // Clear render window each frame before populating again

        if (!isPaused) // Only update physics when simulation isn't paused
        {
            body2.AttractBody(body1);

            body1.Update(deltaTime);
            body2.Update(deltaTime);

            bodyText.setPosX(static_cast<int>(body1.position.x));
            bodyText.setPosY(static_cast<int>(body1.position.y + body1.radius) + 10);

            bodyText.Update();

            if (isMouseDown) // If mouse is clicked, apply force to body1 directly proportional to distance between the body and the mouse
            {
                double cursorDist = (cursorPos - body1.position).GetMagnitude();
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                DrawCircle(renderer, cursorPos.x, cursorPos.y, cursorDist * 0.2); // Display circle around cursor with a radius proportional to the distance between the cursor and the body
                body1.ApplyForce((cursorPos - body1.position) * 25);
            }
        }

        if (isPaused)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0.1);
            SDL_RenderFillRect(renderer, &pausedRect);
            pausedText.Draw(camera, screenSize);
        }

        body2.Draw(renderer, camera, screenSize);
        body1.Draw(renderer, camera, screenSize);

        bodyText.Draw(camera, screenSize);

        // Draw UI on top of scene
        mainWindow->Draw(renderer);

        // Draw cursor
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        DrawCircle(renderer, cursorPos.x, cursorPos.y, 5);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Close SDL_ttf fonts
    TTF_CloseFont(pausedFont);
    TTF_CloseFont(bodyFont);
    TTF_Quit();

    SDL_Quit();
    return 0;
}