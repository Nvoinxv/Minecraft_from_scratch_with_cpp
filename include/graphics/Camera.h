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

    // Gerak horizontal (WASD) + fisika vertikal (gravity, jump)
    void ProcessKeyboard(
        bool forward,
        bool backward,
        bool left,
        bool right,
        bool jump,
        float deltaTime,
        World* world);

    void ProcessMouse(
        float xoffset,
        float yoffset);

    glm::mat4 GetViewMatrix();

private:

    void UpdateVectors();
    bool CheckCollision(const glm::vec3& pos, World* world) const;

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

    // Fisika
    float VelocityY   = 0.0f;   // Kecepatan vertikal saat ini (m/s)
    bool  OnGround    = false;   // Apakah pemain sedang berdiri di tanah?

    static constexpr float GRAVITY      = -25.0f;  // Gravitasi (unit/s^2)
    static constexpr float JUMP_FORCE   =  10.0f;  // Dorongan awal saat lompat
    static constexpr float PLAYER_H     =   1.7f;  // Tinggi pemain (mata ke kaki)
    static constexpr float PLAYER_R     =   0.3f;  // Radius horizontal pemain
};

#endif