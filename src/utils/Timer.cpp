#include "OxygenRender/Timer.h"

namespace OxyRender
{
    Timer::Timer() : m_deltaTime(0.0), m_lastFrame(0.0), m_targetFPS(0)
    {
    }

    Timer &Timer::getInstance()
    {
        static Timer timer;
        return timer;
    }

    double Timer::averageFPS(int samples)
    {
        static std::deque<double> history;
        static double sum = 0.0;

        double fps = FPS();
        history.push_back(fps);
        sum += fps;

        if (history.size() > samples)
        {
            sum -= history.front();
            history.pop_front();
        }

        return sum / history.size();
    }

    void Timer::update(Window &window)
    {
        double currentFrame = window.getTime();
        double frameTime = currentFrame - m_lastFrame;

        if (m_targetFPS > 0)
        {
            double targetFrameTime = 1.0 / static_cast<double>(m_targetFPS);

            if (frameTime < targetFrameTime)
            {
                double sleepTime = targetFrameTime - frameTime;

                if (sleepTime > 0.001)
                {
                    std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime * 0.9));
                }

                while ((currentFrame = window.getTime()) - m_lastFrame < targetFrameTime)
                {
                }

                frameTime = currentFrame - m_lastFrame;
            }
        }

        m_deltaTime = frameTime;
        m_lastFrame = currentFrame;
    }

    double Timer::deltaTime()
    {
        return m_deltaTime;
    }

    double Timer::FPS()
    {
        return (m_deltaTime > 0.0) ? 1.0 / m_deltaTime : 0.0;
    }

    double Timer::now(Window &window)
    {
        return window.getTime();
    }
}
