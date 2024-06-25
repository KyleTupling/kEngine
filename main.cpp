#include <SDL2/SDL.h>
#include <string>
#include <sstream>
#include <iostream>
#include <Vector2D/Vector2D.h>
#include <cmath>
#include <deque>

const double G = 6.67e-11; // Universal gravitational constant

bool isRunning = true; // Keep track of engine state
int cursorPosX = 0;
int cursorPosY = 0;
bool isMouseDown = false;

class Body
{
    public:
        int radius = 10;
        double mass = 10;
        bool active = true; // Tracks whether to update instance during simulation

        Vector2D position;
        Vector2D velocity;
        Vector2D acceleration;

        std::deque<Vector2D> previousPositions; // Holds previous positions up to a maximum amount
        int maximumPrevPos = 50;
        bool drawPrevious = false; // Enables drawing of previous positions trail
        int minimumPrevDist = 150; // The minimum distance required between last stored position and current position to allow storing of current position

        Uint8 colour[4] = {0, 0, 0, 255};

        Body()
        {
            this->position.Set(0, 0);
            this->velocity.Set(0, 0);
        }

        Body(Vector2D pos, Vector2D vel)
        {
            this->position.Set(pos.x, pos.y);
            this->velocity.Set(vel.x, vel.y);
        }

        // Applies a force to a body instance based on Newton's 2nd law of motion
        void ApplyForce(Vector2D force)
        {
            // Acceleration = Force / Mass
            Vector2D acc = force.Divide(this->mass);
            this->acceleration = this->acceleration + acc;
        }

        // Attract body instance using Newton's Law of Gravitation
        void AttractBody(Body& body)
        {
            Vector2D dir = Vector2D::Subtract(this->position, body.position);
            Vector2D unitDir = dir.GetUnitVector();
            double dirMag = dir.GetMagnitude();
            double forceMag = (G * this->mass * body.mass) / (dirMag * dirMag);
            Vector2D force = unitDir.Multiply(forceMag);
            body.ApplyForce(force);
        }

        // Updates logic and physics of body instance
        // Takes parameter [double]deltaTime for equations of motion
        void Update(double deltaTime)
        {
            if(this->previousPositions.size() == 0) // If previous position list is empty, add current position
            {
                this->previousPositions.push_back(this->position);
            } 
            else // If previous position list isn't empty, check that enough distance has been made between last position to add current
            {
                if(abs((this->position - this->previousPositions.back()).GetMagnitudeSqr()) > this->minimumPrevDist)
                {
                    this->previousPositions.push_back(this->position);
                }
            }
            
            // Ensure previous position list doesn't store more than maximum amount of positions
            if(this->previousPositions.size() > this->maximumPrevPos)
            {
                this->previousPositions.pop_front();
            }

            this->velocity = this->velocity + this->acceleration * deltaTime;
            this->position = this->position + this->velocity * deltaTime;
            this->acceleration.Set(0, 0);
        }

        // Draws the body instance to a window using the window's assigned renderer
        // Takes parameter [SDL_Renderer*]renderer
        void Draw(SDL_Renderer* renderer)
        {
            // Check whether to draw previous positions
            if(this->drawPrevious)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
                for(int i = 0; i < this->previousPositions.size(); i++)
                {
                    SDL_Rect rect{static_cast<int>(this->previousPositions[i].x) - 1, static_cast<int>(this->previousPositions[i].y) - 1, 2, 2}; // TODO: Fix double->int conversion
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
            SDL_SetRenderDrawColor(renderer, this->colour[0], this->colour[1], this->colour[2], this->colour[3]);
            SDL_Rect bodyRect{(int)this->position.x - this->radius, (int)this->position.y - this->radius, this->radius * 2, this->radius * 2};
            SDL_RenderFillRect(renderer, &bodyRect);
        }
};

// Convert [double] parameter x to [string]
std::string to_string(double x)
{
  std::ostringstream ss;
  ss << x;
  return ss.str();
}


// Temporary procedure from SE
void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
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

int main(int argv, char** args)
{
    // Initialise variables related to deltaTime
    Uint64 current = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    double deltaTime = 0;

    Body body1(Vector2D(75, 50), Vector2D(75, 0));
    body1.colour[2] = 255;
    body1.drawPrevious = true;

    Body body2(Vector2D(500, 300), Vector2D(0, 0));
    body2.radius = 15;
    body2.colour[0] = 230;
    body2.colour[1] = 180;
    body2.colour[2] = 30;
    body2.mass = 1e18;

    // Initialise SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_ShowCursor(SDL_DISABLE);

    // TODO: Create Engine class to manage SDL window, renderer etc.
    SDL_Window* window = SDL_CreateWindow("kEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    while(isRunning)
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
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {  
                isRunning = false;
                break;
            }

            if(event.type == SDL_MOUSEMOTION)
            {
                // Store current mouse position on mouse move
                cursorPosX = event.motion.x;
                cursorPosY = event.motion.y;
            }

            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                isMouseDown = true;
            }

            if(event.type == SDL_MOUSEBUTTONUP)
            {
                isMouseDown = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer); // Clear render window each frame before populating again

        body2.AttractBody(body1);
        body2.Draw(renderer);

        body1.Update(deltaTime);
        if(isMouseDown) // If mouse is clicked, apply force to body1 directly proportional to distance between the body and the mouse
        {
            body1.ApplyForce((Vector2D(cursorPosX, cursorPosY) - body1.position) * 25);
        }
        body1.Draw(renderer);

        // Draw cursor
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        DrawCircle(renderer, cursorPosX, cursorPosY, 5);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}