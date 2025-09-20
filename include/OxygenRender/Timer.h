#pragma once
#include "OxygenRender/Window.h"
#include <thread>
#include <chrono>
#include <deque>

namespace OxyRender
{
    class Timer
    {
    private:
        double m_lastTime = 0.0;
        double m_deltaTime = 0.0;
        double m_accumulator = 0.0;
        int m_targetFPS = 0;
        double m_totalTime = 0.0;
        Timer();
        ~Timer() = default;

    public:
        Timer(const Timer &) = delete;
        Timer &operator=(const Timer &) = delete;
        static Timer &getInstance();

        void update(Window &window);

        double deltaTime();
        double FPS();
        double now(Window &window);

        double averageFPS(int samples);
        double totalTime() const { return m_totalTime; }
        void setTargetFPS(int fps) { m_targetFPS = fps; }
        int getTargetFPS() const { return m_targetFPS; }
    };
}
