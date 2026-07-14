#include "include/graphics/Camera.h"

// Ini titik spawn kamera di dunia game
Camera::Camera(glm::vec3 spawnPosition)
{
    // Set posisi awal
    // Gw buat rada acak
    Position = spawnPosition;
    
    // Set arah kamera menghadap ke depan
    Front = glm::vec3(0.0f, 0.0f, -1.0f);
    
    // Set arah atas kamera
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    Yaw = -90.0f;
    Pitch = 0.0f;

    Speed = 8.0f;
    Sensitivity = 0.1f;

    UpdateVectors();
}

// Render kamera berdasarkan input keyboard
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(
        Position,
        Position + Front,
        Up
    );
}

// Bagian posisi mouse 
void Camera::ProcessMouse(float xoffset, float yoffset)
{
    xoffset *= Sensitivity;
    yoffset *= Sensitivity;

    Yaw += xoffset;

    Pitch += yoffset;

    if (Pitch > 89.0f)
        Pitch = 89.0f;

    if (Pitch < -89.0f)
        Pitch = -89.0f;

    UpdateVectors();
}

// Mengubah posisi kamera berdasarkan pemain lakukan
void Camera::UpdateVectors()
{
    glm::vec3 front;

    front.x =
        cos(glm::radians(Yaw))
        * cos(glm::radians(Pitch));

    front.y =
        sin(glm::radians(Pitch));

    front.z =
        sin(glm::radians(Yaw))
        * cos(glm::radians(Pitch));

    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(
        Front,
        WorldUp));

    Up = glm::normalize(glm::cross(
        Right,
        Front));
}

// Bagian melakukan pergerakan kamera berdasarkan input keyboard
void Camera::ProcessKeyboard(
    bool forward,
    bool backward,
    bool left,
    bool right,
    float deltaTime)
{
    float velocity = Speed * deltaTime;

    if (forward)
        Position += Front * velocity;

    if (backward)
        Position -= Front * velocity;

    if (left)
        Position -= Right * velocity;

    if (right)
        Position += Right * velocity;
}