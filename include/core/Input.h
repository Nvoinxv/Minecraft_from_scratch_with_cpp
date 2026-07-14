#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Input
{
public:
    //==================================================
    // Initialization
    //==================================================

    static void Initialize(GLFWwindow* window);
    static void Update();

    //==================================================
    // Keyboard
    //==================================================

    static bool IsKeyPressed(int key);
    static bool IsKeyDown(int key);
    static bool IsKeyReleased(int key);

    //==================================================
    // Mouse Buttons
    //==================================================

    static bool IsMousePressed(int button);
    static bool IsMouseDown(int button);
    static bool IsMouseReleased(int button);

    //==================================================
    // Mouse Position
    //==================================================

    static double GetMouseX();
    static double GetMouseY();

    static double GetDeltaX();
    static double GetDeltaY();

    //==================================================
    // Mouse Scroll
    //==================================================

    static double GetScrollX();
    static double GetScrollY();

private:

    static GLFWwindow* s_Window;

    static bool s_CurrentKeys[GLFW_KEY_LAST + 1];
    static bool s_PreviousKeys[GLFW_KEY_LAST + 1];

    static bool s_CurrentMouse[GLFW_MOUSE_BUTTON_LAST + 1];
    static bool s_PreviousMouse[GLFW_MOUSE_BUTTON_LAST + 1];

    static double s_MouseX;
    static double s_MouseY;

    static double s_LastMouseX;
    static double s_LastMouseY;

    static double s_DeltaX;
    static double s_DeltaY;

    static double s_ScrollX;
    static double s_ScrollY;

private:

    static void ScrollCallback(
        GLFWwindow* window,
        double xOffset,
        double yOffset
    );
};