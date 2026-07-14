#include "graphics/Camera.h"

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

#include "world/World.h"

// Bagian melakukan pergerakan kamera berdasarkan input keyboard
void Camera::ProcessKeyboard(
    bool forward,
    bool backward,
    bool left,
    bool right,
    float deltaTime,
    World* world)
{
    float velocity = Speed * deltaTime;

    glm::vec3 movement(0.0f);
    if (forward)  movement += Front * velocity;
    if (backward) movement -= Front * velocity;
    if (left)     movement -= Right * velocity;
    if (right)    movement += Right * velocity;

    if (!world) {
        Position += movement; // Fallback jika world null
        return;
    }

    // Fungsi helper untuk mengecek tabrakan AABB pemain di posisi target
    auto checkCollision = [&](const glm::vec3& targetPos) -> bool {
        // Asumsi ukuran pemain: Lebar 0.6 (-0.3 s.d 0.3), Tinggi 1.8 (-1.6 s.d +0.2 dari mata/kamera)
        float minX = targetPos.x - 0.3f;
        float maxX = targetPos.x + 0.3f;
        float minY = targetPos.y - 1.6f;
        float maxY = targetPos.y + 0.2f;
        float minZ = targetPos.z - 0.3f;
        float maxZ = targetPos.z + 0.3f;

        // Cek semua blok integer yang bersentuhan dengan AABB
        int startX = static_cast<int>(std::floor(minX));
        int endX   = static_cast<int>(std::floor(maxX));
        int startY = static_cast<int>(std::floor(minY));
        int endY   = static_cast<int>(std::floor(maxY));
        int startZ = static_cast<int>(std::floor(minZ));
        int endZ   = static_cast<int>(std::floor(maxZ));

        for (int x = startX; x <= endX; ++x) {
            for (int y = startY; y <= endY; ++y) {
                for (int z = startZ; z <= endZ; ++z) {
                    // ID 0 adalah udara (Air). Selain udara dianggap solid (tembok/tanah).
                    if (world->GetBlockGlobal(x, y, z) != 0) {
                        return true; // Ada tabrakan
                    }
                }
            }
        }
        return false;
    };

    // Aplikasikan pergerakan per-sumbu agar bisa 'meluncur' (sliding) di tembok
    Position.x += movement.x;
    if (checkCollision(Position)) {
        Position.x -= movement.x; // Batalkan gerakan X jika menabrak
    }

    Position.y += movement.y;
    if (checkCollision(Position)) {
        Position.y -= movement.y; // Batalkan gerakan Y jika menabrak
    }

    Position.z += movement.z;
    if (checkCollision(Position)) {
        Position.z -= movement.z; // Batalkan gerakan Z jika menabrak
    }
}