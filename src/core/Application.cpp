#include "core/Application.h"

#include <GLFW/glfw3.h>

Application::Application()
    :
    m_Window(
        1280,
        720,
        "Minecraft Alpha"),
    m_Camera(glm::vec3(
        0.0f,
        64.0f,
        0.0f)),
    m_IsRunning(false)
{
}

Application::~Application()
{
    std::cout
        << "[Application] Destructor called."
        << std::endl;

    Shutdown();
}

bool Application::Initialize()
{
    std::cout
        << "[Application] Initializing..."
        << std::endl;

    if (!m_Window.Create())
    {
        std::cout
            << "[Application] Failed to create window."
            << std::endl;

        return false;
    }

    std::cout
        << "[Application] Window created."
        << std::endl;

    Time::Init();

    std::cout
        << "[Application] Time initialized."
        << std::endl;

    Input::Initialize(
        m_Window.GetNativeWindow());

    std::cout
        << "[Application] Input initialized."
        << std::endl;

    if (!m_Renderer.Initialize())
    {
        std::cout
            << "[Application] Renderer initialization failed."
            << std::endl;

        return false;
    }

    std::cout
        << "[Application] Renderer initialized."
        << std::endl;

    m_IsRunning = true;

    std::cout
        << "[Application] Initialization complete."
        << std::endl;

    return true;
}

void Application::Run()
{
    std::cout
        << "[Application] Entering main loop."
        << std::endl;

    while (m_IsRunning &&
           !m_Window.ShouldClose())
    {
        Time::Update();

        m_Window.PollEvents();

        Input::Update();

        Update();

        Render();

        m_Window.SwapBuffers();
    }

    std::cout
        << "[Application] Main loop finished."
        << std::endl;
}

void Application::Update()
{
    m_Camera.ProcessKeyboard(
        Input::IsKeyDown(GLFW_KEY_W),
        Input::IsKeyDown(GLFW_KEY_S),
        Input::IsKeyDown(GLFW_KEY_A),
        Input::IsKeyDown(GLFW_KEY_D),
        Time::GetDeltaTime()
    );

    m_Camera.ProcessMouse(
        static_cast<float>(Input::GetDeltaX()),
        static_cast<float>(Input::GetDeltaY())
    );

    //---------------------------------------
    // World
    //---------------------------------------

    // m_World.Update();

    //---------------------------------------
    // Entity
    //---------------------------------------

    // m_Player.Update();
}

void Application::Render()
{
    m_Renderer.BeginFrame();

    //---------------------------------------
    // World
    //---------------------------------------

    // m_Renderer.DrawWorld(m_World);

    //---------------------------------------
    // Player
    //---------------------------------------

    //---------------------------------------
    // UI
    //---------------------------------------

    m_Renderer.EndFrame();
}

void Application::Shutdown()
{
    if (!m_IsRunning)
    {
        std::cout
            << "[Application] Shutdown skipped."
            << std::endl;

        return;
    }

    std::cout
        << "[Application] Shutting down..."
        << std::endl;

    m_IsRunning = false;

    m_Renderer.Shutdown();

    m_Window.Destroy();

    std::cout
        << "[Application] Shutdown complete."
        << std::endl;
}