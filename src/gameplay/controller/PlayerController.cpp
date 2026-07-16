#include "gameplay/controller/PlayerController.h"
#include "graphics/Camera.h"
#include "core/Input.h"
#include "world/World.h"

#include <GLFW/glfw3.h>

PlayerController::PlayerController(Camera* camera)
    : m_Camera(camera)
{
}

void PlayerController::Update(float deltaTime, World* world)
{
    if (m_Camera == nullptr)
        return;

    HandleKeyboard(deltaTime, world);
    HandleMouse();
}

void PlayerController::SetCamera(Camera* camera)
{
    m_Camera = camera;
}

Camera* PlayerController::GetCamera() const
{
    return m_Camera;
}

void PlayerController::HandleKeyboard(float deltaTime, World* world)
{
    const bool forward  = Input::IsKeyPressed(GLFW_KEY_W);
    const bool backward = Input::IsKeyPressed(GLFW_KEY_S);
    const bool left     = Input::IsKeyPressed(GLFW_KEY_A);
    const bool right    = Input::IsKeyPressed(GLFW_KEY_D);
    const bool jump     = Input::IsKeyPressed(GLFW_KEY_SPACE);

    m_Camera->ProcessKeyboard(
        forward,
        backward,
        left,
        right,
        jump,
        deltaTime,
        world
    );
}

void PlayerController::HandleMouse()
{
    const float mouseX = (float)Input::GetDeltaX();
    const float mouseY = (float)Input::GetDeltaY();

    m_Camera->ProcessMouse(mouseX, mouseY);
}