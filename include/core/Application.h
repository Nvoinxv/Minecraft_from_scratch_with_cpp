#pragma once

#include "Window.h"
#include "Time.h"
#include "Input.h"

#include "../graphics/Renderer.h"
#include "../graphics/Camera.h"
#include "../graphics/Shader.h"
#include "../world/World.h"
#include "../world/Block.h"


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

    void UpdateTelemetry();

    void PrintTelemetryLog(bool forceImmediate = false);

private:

    Window      m_Window;
    Renderer    m_Renderer;
    Camera      m_Camera;
    Shader m_Shader;
    Shader m_CrosshairShader;

    GLuint m_CrosshairVAO = 0;
    GLuint m_CrosshairVBO = 0;
    World       m_World;

    bool        m_IsRunning;
    float       m_TelemetryTimer;
};