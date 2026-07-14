#pragma once

#include "Window.h"
#include "Time.h"
#include "Input.h"

#include "../graphics/Renderer.h"
#include "../graphics/Camera.h"

class Application
{
public:

    Application();

    ~Application();

    bool Initialize();

    void Run();

    void Shutdown();

private:

    void Update();

    void Render();

private:

    Window      m_Window;
    Renderer    m_Renderer;
    Camera      m_Camera;

    bool        m_IsRunning;
};