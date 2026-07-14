#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <iomanip>

class World;

class Camera
{
public:

    Camera(glm::vec3 spawnPosition);

    void ProcessKeyboard(
        bool forward,
        bool backward,
        bool left,
        bool right,
        float deltaTime,
        World* world);

    void ProcessMouse(
        float xoffset,
        float yoffset);

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