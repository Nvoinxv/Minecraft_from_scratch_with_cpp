#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct GLFWwindow;

class Window
{
public:
    Window(int width,
           int height,
           const std::string& title);

    ~Window();

    bool Create();
    void Destroy();

    void PollEvents();
    void SwapBuffers();

    bool ShouldClose() const;

    GLFWwindow* GetNativeWindow() const;

    int GetWidth() const;
    int GetHeight() const;

    void SetTitle(const std::string& title);

    void SetVSync(bool enabled);

private:
    GLFWwindow* m_Window;

    int m_Width;
    int m_Height;

    std::string m_Title;

private:
    bool InitializeGLFW();
    bool CreateWindow();
};

#endif