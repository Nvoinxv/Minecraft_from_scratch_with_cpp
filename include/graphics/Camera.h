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
    // Mode saat ini. Default false (survival). Set true dari luar (Application/GameMode)
    // untuk mengaktifkan lompat ganda di udara.
    bool IsCreative = false;

private:

    void UpdateVectors();
    bool CheckCollision(const glm::vec3& pos, World* world, float feetEpsilon = 0.0f) const;

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
    
    int m_AirJumpsUsed = 0;              // Berapa kali lompat tambahan sudah dipakai selagi di udara
    static constexpr int MAX_AIR_JUMPS = 1; // Creative: 1x lompat tambahan di udara (total 2x tekan space)

    static constexpr float GRAVITY      = -25.0f;  // Gravitasi (unit/s^2)
    static constexpr float JUMP_FORCE   =  10.0f;  // Dorongan awal saat lompat
    static constexpr float TERMINAL_VELOCITY = -40.0f; // Batas kecepatan jatuh maksimal, mencegah gap collision membesar tanpa batas
    static constexpr float PLAYER_H     =   1.7f;  // Tinggi pemain (mata ke kaki)
    static constexpr float PLAYER_R     =   0.3f;  // Radius horizontal pemain
};

#endif