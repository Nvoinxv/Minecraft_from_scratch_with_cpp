#include "core/Input.h"

#include <cstring>

GLFWwindow* Input::s_Window = nullptr;

bool Input::s_CurrentKeys[GLFW_KEY_LAST + 1]{};
bool Input::s_PreviousKeys[GLFW_KEY_LAST + 1]{};

bool Input::s_CurrentMouse[GLFW_MOUSE_BUTTON_LAST + 1]{};
bool Input::s_PreviousMouse[GLFW_MOUSE_BUTTON_LAST + 1]{};

double Input::s_MouseX = 0.0;
double Input::s_MouseY = 0.0;

double Input::s_LastMouseX = 0.0;
double Input::s_LastMouseY = 0.0;

double Input::s_DeltaX = 0.0;
double Input::s_DeltaY = 0.0;

double Input::s_ScrollX = 0.0;
double Input::s_ScrollY = 0.0;

static bool s_FirstMouse = true;

void Input::Initialize(GLFWwindow* window)
{
    s_Window = window;

    if (s_Window == nullptr)
    {
        return;
    }

    glfwSetInputMode(s_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(s_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    glfwSetScrollCallback(s_Window, ScrollCallback);
    glfwSetCursorPosCallback(s_Window, MouseCallback);

    glfwGetCursorPos(
        s_Window,
        &s_MouseX,
        &s_MouseY
    );
    s_LastMouseX = s_MouseX;
    s_LastMouseY = s_MouseY;
    s_FirstMouse = true;
    s_DeltaX = 0.0;
    s_DeltaY = 0.0;
}

void Input::Update()
{
    if (s_Window == nullptr)
    {
        return;
    }

    // Scroll hanya berlaku satu frame
    s_ScrollX = 0.0;
    s_ScrollY = 0.0;

    std::memcpy(
        s_PreviousKeys,
        s_CurrentKeys,
        sizeof(s_CurrentKeys)
    );

    std::memcpy(
        s_PreviousMouse,
        s_CurrentMouse,
        sizeof(s_CurrentMouse)
    );

    for (int key = 0; key <= GLFW_KEY_LAST; key++)
    {
        s_CurrentKeys[key] =
            glfwGetKey(s_Window, key) == GLFW_PRESS;
    }

    for (int button = 0; button <= GLFW_MOUSE_BUTTON_LAST; button++)
    {
        s_CurrentMouse[button] =
            glfwGetMouseButton(
                s_Window,
                button
            ) == GLFW_PRESS;
    }

    // glfwGetCursorPos now handled by MouseCallback
    // but we still need to calculate Delta and reset it if no movement happened
    // Wait, with MouseCallback, Delta should be accumulated and then reset after use.
    // For now, let's just clear DeltaX/Y at the START of next frame if no callback happened,
    // actually it's easier to just calculate delta from s_MouseX here.

    if (s_FirstMouse)
    {
        s_LastMouseX = s_MouseX;
        s_LastMouseY = s_MouseY;
        s_FirstMouse = false;
    }

    s_DeltaX = s_MouseX - s_LastMouseX;
    s_DeltaY = s_MouseY - s_LastMouseY;

    s_LastMouseX = s_MouseX;
    s_LastMouseY = s_MouseY;
}

bool Input::IsKeyPressed(int key)
{
    if (key < 0 || key > GLFW_KEY_LAST)
    {
        return false;
    }

    return
        s_CurrentKeys[key] &&
       !s_PreviousKeys[key];
}

bool Input::IsKeyDown(int key)
{
    if (key < 0 || key > GLFW_KEY_LAST)
    {
        return false;
    }

    return s_CurrentKeys[key];
}

bool Input::IsKeyReleased(int key)
{
    if (key < 0 || key > GLFW_KEY_LAST)
    {
        return false;
    }

    return
       !s_CurrentKeys[key] &&
        s_PreviousKeys[key];
}

bool Input::IsMousePressed(int button)
{
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
    {
        return false;
    }

    return
        s_CurrentMouse[button] &&
       !s_PreviousMouse[button];
}

bool Input::IsMouseDown(int button)
{
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
    {
        return false;
    }

    return s_CurrentMouse[button];
}

bool Input::IsMouseReleased(int button)
{
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
    {
        return false;
    }

    return
       !s_CurrentMouse[button] &&
        s_PreviousMouse[button];
}

double Input::GetMouseX()
{
    return s_MouseX;
}

double Input::GetMouseY()
{
    return s_MouseY;
}

double Input::GetDeltaX()
{
    return s_DeltaX;
}

double Input::GetDeltaY()
{
    return s_DeltaY;
}

double Input::GetScrollX()
{
    return s_ScrollX;
}

double Input::GetScrollY()
{
    return s_ScrollY;
}

void Input::ScrollCallback(
    GLFWwindow*,
    double xOffset,
    double yOffset)
{
    s_ScrollX = xOffset;
    s_ScrollY = yOffset;
}

void Input::MouseCallback(
    GLFWwindow*,
    double xpos,
    double ypos)
{
    s_MouseX = xpos;
    s_MouseY = ypos;
}