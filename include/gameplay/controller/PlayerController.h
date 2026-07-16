#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "../graphics/Camera.h"

class Camera;
class World;

class PlayerController
{
public:

    PlayerController(Camera* camera);

    void Update(float deltaTime, World* world);

    void SetCamera(Camera* camera);

    Camera* GetCamera() const;

private:

    void HandleKeyboard(float deltaTime, World* world);
    void HandleMouse();

private:

    Camera* m_Camera = nullptr;
};

#endif