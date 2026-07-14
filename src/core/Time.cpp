#include "core/Time.h"
#include <GLFW/glfw3.h>

float Time::s_DeltaTime = 0.0f;

float Time::s_LastFrame = 0.0f;
float Time::s_CurrentFrame = 0.0f;

float Time::s_FPS = 0.0f;
float Time::s_FPSTimer = 0.0f;

unsigned int Time::s_FrameCounter = 0;
unsigned long long Time::s_TotalFrames = 0;

void Time::Init()
{
    s_LastFrame = static_cast<float>(glfwGetTime());
    s_CurrentFrame = s_LastFrame;

    s_DeltaTime = 0.0f;

    s_FPSTimer = 0.0f;
    s_FrameCounter = 0;
    s_TotalFrames = 0;
}

void Time::Update()
{
    s_CurrentFrame = static_cast<float>(glfwGetTime());

    s_DeltaTime = s_CurrentFrame - s_LastFrame;

    s_LastFrame = s_CurrentFrame;

    s_TotalFrames++;
    s_FrameCounter++;

    s_FPSTimer += s_DeltaTime;

    if (s_FPSTimer >= 1.0f)
    {
        s_FPS = static_cast<float>(s_FrameCounter);

        s_FrameCounter = 0;
        s_FPSTimer = 0.0f;
    }
}

float Time::GetDeltaTime()
{
    return s_DeltaTime;
}

float Time::GetTime()
{
    return s_CurrentFrame;
}

unsigned long long Time::GetFrameCount()
{
    return s_TotalFrames;
}

float Time::GetFPS()
{
    return s_FPS;
}