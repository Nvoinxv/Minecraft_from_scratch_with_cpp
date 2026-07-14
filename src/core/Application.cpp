#include "core/Application.h"

#include <GLFW/glfw3.h>

Application::Application()
    :
    m_Camera(glm::vec3(
        0.0f,
        64.0f,
        0.0f)),
    m_IsRunning(false)
{
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
        return;
    }

    m_IsRunning = false;

    m_Renderer.Shutdown();

    m_Window.Destroy();
}