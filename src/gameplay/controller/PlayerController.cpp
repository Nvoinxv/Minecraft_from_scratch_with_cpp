#include "gameplay/PlayerController.h"

#include "graphics/Camera.h"
#include "core/Input.h"
#include "world/World.h"

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
    const bool forward  = Input::IsKeyPressed(KEY_W);
    const bool backward = Input::IsKeyPressed(KEY_S);
    const bool left     = Input::IsKeyPressed(KEY_A);
    const bool right    = Input::IsKeyPressed(KEY_D);
    const bool jump     = Input::IsKeyPressed(KEY_SPACE);

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
    const float mouseX = Input::GetMouseDeltaX();
    const float mouseY = Input::GetMouseDeltaY();

    m_Camera->ProcessMouse(mouseX, mouseY);
}