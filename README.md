# kEngine

C++ physics engine in early development. Currently testing planetary orbits.

<!-- ![image](https://github.com/user-attachments/assets/604c4328-49fb-49e2-b01a-8125465c91d2) -->

![image](https://github.com/user-attachments/assets/691e7db4-77f9-4689-96f0-a81afa03c084)

## Creating an application
Specific engine applications can be made by inheriting from the [Application class](https://github.com/KyleTupling/kEngine/blob/master/kEngine/Application.h).

```cpp
#include "Application.h"

class TestApplication : public Application
{
public:
    TestApplication(const ApplicationConfig& config);
    ~TestApplication();

    void HandleEvents() override;
    void Update(double deltaTime) override;
    void Render() override;
};
```

Creating and running the application, as seen in [main.cpp](https://github.com/KyleTupling/kEngine/blob/master/kEngine/main.cpp) can be done as follows:

```cpp
#include "TestApplication.h"
#include "Vector2D.h"

ApplicationConfig config;
config.Name = "Test Application";                    // This will display in the application window titlebar
config.ScreenSize = Vector2D(1280, 720);             // This will be the size of the application window
config.DrawCursor = true;

TestApplication* app = new TestApplication(config);
app->Run();
Application::Shutdown();
```
