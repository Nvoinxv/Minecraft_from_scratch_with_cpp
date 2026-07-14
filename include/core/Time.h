#ifndef TIME_H
#define TIME_H

class Time
{
public:
    // Dipanggil sekali saat program dimulai
    static void Init();

    // Dipanggil setiap frame
    static void Update();

    // Delta time (detik)
    static float GetDeltaTime();

    // Waktu sejak game berjalan (detik)
    static float GetTime();

    // Frame sekarang
    static unsigned long long GetFrameCount();

    // FPS
    static float GetFPS();

private:
    static float s_DeltaTime;
    static float s_LastFrame;
    static float s_CurrentFrame;

    static float s_FPS;
    static float s_FPSTimer;

    static unsigned int s_FrameCounter;
    static unsigned long long s_TotalFrames;
};

#endif