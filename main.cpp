// Bagian core
#include "../include/core/Window.h"
#include "../include/core/Time.h"
#include "../include/core/Application.h"

// Bagian graphics
#include "../include/graphics/Camera.h"

#include <GLFW/glfw3.h>

int core_implementation()
{
    Window window(1280, 720, "Minecraft From Scratch By Nvoin");

    if (!window.Create())
        return -1;

    window.SetVSync(true);

    Camera camera(glm::vec3(0.0f, 64.0f, 0.0f));

    Time::Init();

    float printTimer = 0.0f;

    while (!window.ShouldClose())
    {
        Time::Update();

        window.PollEvents();

        // ===== Input =====

        camera.ProcessKeyboard(
            glfwGetKey(window.GetNativeWindow(), GLFW_KEY_W) == GLFW_PRESS,
            glfwGetKey(window.GetNativeWindow(), GLFW_KEY_S) == GLFW_PRESS,
            glfwGetKey(window.GetNativeWindow(), GLFW_KEY_A) == GLFW_PRESS,
            glfwGetKey(window.GetNativeWindow(), GLFW_KEY_D) == GLFW_PRESS,
            Time::GetDeltaTime()
        );

        // ===== Update =====

        glm::mat4 view = camera.GetViewMatrix();

        // ===== Debug =====

        printTimer += Time::GetDeltaTime();

        if (printTimer >= 1.0f)
        {
            printTimer = 0.0f;

            std::cout << std::fixed << std::setprecision(2);

            std::cout
                << "Time : " << Time::GetTime()
                << " | Delta : " << Time::GetDeltaTime()
                << " | FPS : " << Time::GetFPS()
                << " | Frame : " << Time::GetFrameCount()
                << '\n';

            std::cout
                << "Camera Position : ("
                << camera.Position.x << ", "
                << camera.Position.y << ", "
                << camera.Position.z << ")"
                << '\n';

            std::cout
                << "Camera Front : ("
                << camera.Front.x << ", "
                << camera.Front.y << ", "
                << camera.Front.z << ")"
                << "\n\n";
        }

        // ===== Render =====

        window.SwapBuffers();
    }

    return 0;
}

int main() {
    return core_implementation();
    return 0;
}