#include "core/Application.h"

#include <GLFW/glfw3.h>

Application::Application()
    :
    m_IsRunning(false)
{
}

Application::~Application()
{
    Shutdown();
}

bool Application::Initialize()
{
    if (!m_Window.Create(
            1280,
            720,
            "Minecraft Alpha"))
    {
        return false;
    }

    Input::Initialize(
        m_Window.GetNativeWindow());

    if (!m_Renderer.Initialize())
    {
        return false;
    }

    m_IsRunning = true;

    return true;
}

void Application::Run()
{
    while (m_IsRunning &&
           !m_Window.ShouldClose())
    {
        Time::Update();

        glfwPollEvents();

        Input::Update();

        Update();

        Render();

        m_Window.Update();
    }
}

void Application::Update()
{
    //---------------------------------------
    // Camera
    //---------------------------------------

    m_Camera.Update();

    //---------------------------------------
    // World
    //---------------------------------------

    // m_World.Update();

    //---------------------------------------
    // Entity
    //---------------------------------------

    // m_Player.Update();

    //---------------------------------------
    // Physics
    //---------------------------------------

    //---------------------------------------
    // Audio
    //---------------------------------------
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
        return;
    }

    m_IsRunning = false;

    m_Renderer.Shutdown();

    m_Window.Destroy();
}