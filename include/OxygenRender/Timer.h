#pragma once
#include "OxygenRender/Window.h"
#include <thread>
#include <chrono>
#include <deque> 

namespace OxyRender
{
    // 计时器单例类
    class Timer
    {
    private:
        double m_lastFrame = 0.0;
        double m_deltaTime = 0.0;
        int m_targetFPS = 0; // 0 表示不锁帧
        Timer();
        ~Timer() = default;

    public:
        Timer(const Timer &) = delete;
        Timer &operator=(const Timer &) = delete;
        static Timer &getInstance();
        double averageFPS(int samples);
        void update(Window &window);

        double deltaTime();
        double FPS();
        double now(Window &window);

        void setTargetFPS(int fps) { m_targetFPS = fps; }
        int targetFPS() const { return m_targetFPS; }
    };
}
