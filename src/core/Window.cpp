#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/Window.h"

Window::Window(int width,
               int height,
               const std::string& title)
    : m_Window(nullptr),
      m_Width(width),
      m_Height(height),
      m_Title(title)
{
}

Window::~Window()
{
    Destroy();
}

bool Window::Create()
{
    if (!InitializeGLFW())
        return false;

    if (!CreateWindow())
        return false;

    return true;
}

bool Window::InitializeGLFW()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW.\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_CORE_PROFILE);

    return true;
}

bool Window::CreateWindow()
{
    m_Window = glfwCreateWindow(
        m_Width,
        m_Height,
        m_Title.c_str(),
        nullptr,
        nullptr);

    if (m_Window == nullptr)
    {
        std::cerr << "Failed to create GLFW Window.\n";

        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader(
            (GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD.\n";

        glfwDestroyWindow(m_Window);
        glfwTerminate();

        return false;
    }

    glViewport(
        0,
        0,
        m_Width,
        m_Height);

    return true;
}

void Window::Destroy()
{
    if (m_Window != nullptr)
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }

    glfwTerminate();
}

void Window::PollEvents()
{
    glfwPollEvents();
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_Window);
}

GLFWwindow* Window::GetNativeWindow() const
{
    return m_Window;
}

int Window::GetWidth() const
{
    return m_Width;
}

int Window::GetHeight() const
{
    return m_Height;
}

void Window::SetTitle(const std::string& title)
{
    m_Title = title;

    glfwSetWindowTitle(
        m_Window,
        m_Title.c_str());
}

void Window::SetVSync(bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
}