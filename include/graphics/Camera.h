#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
    Camera();

    void ProcessKeyboard(bool forward,
                         bool backward,
                         bool left,
                         bool right,
                         float deltaTime);

    void ProcessMouse(float xoffset, float yoffset);

    glm::mat4 GetViewMatrix();

private:

    void UpdateVectors();

public:

    glm::vec3 Position;

    glm::vec3 Front;

    glm::vec3 Up;

    glm::vec3 Right;

    glm::vec3 WorldUp;

    float Yaw;

    float Pitch;

    float Speed;

    float Sensitivity;
};

#endif