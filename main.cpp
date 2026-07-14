// Bagian core
#include "../include/core/Window.h"
#include "../include/core/TIme.h"

// Bagian graphics
#include "../include/graphics/Camera.h"

void core_implementation() {
    Window window(1280, 720, "Minecraft From Scratch By Nvoin");

    if (!window.Create())
        return -1;

    window.SetVSync(true);

    while (!window.ShouldClose())
    {
        window.PollEvents();

        // ===== Input =====

        // ===== Update =====

        // ===== Render =====

        window.SwapBuffers();
    }
}
void graphics_implementation() {
    // Melakukan implementasi pada kamera
    Camera cam;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.ProcessKeyboard(
            glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS,
            deltaTime
        );

        glm::mat4 view = camera.GetViewMatrix();
    }
}

int main() {

    // Melakukan implementasi pada bagian core
    // Implementasi pembuatan window


}