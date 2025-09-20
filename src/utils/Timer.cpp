#include "OxygenRender/Timer.h"

namespace OxyRender
{
    Timer::Timer() : m_deltaTime(0.0), m_lastTime(0.0), m_accumulator(0.0), m_targetFPS(0) {}

    Timer &Timer::getInstance()
    {
        static Timer timer;
        return timer;
    }

    void Timer::update(Window &window)
    {
        double currentTime = window.getTime();
        double frameTime = currentTime - m_lastTime;
        m_lastTime = currentTime;
        m_totalTime += frameTime;
        m_accumulator += frameTime;

        if (m_targetFPS > 0)
        {
            double targetFrameTime = 1.0 / m_targetFPS;

            if (m_accumulator < targetFrameTime)
            {
                double sleepTime = targetFrameTime - m_accumulator;

                if (sleepTime > 0.001)
                {
                    window.waitEventsTimeout(sleepTime - 0.001);
                }

                while ((window.getTime() - currentTime) < sleepTime)
                {
                }

                double afterWait = window.getTime();
                frameTime = afterWait - m_lastTime + m_accumulator;
                m_lastTime = afterWait;
                m_accumulator = targetFrameTime;
            }

            m_deltaTime = targetFrameTime;
            m_accumulator -= targetFrameTime;
        }
        else
        {
            m_deltaTime = frameTime;
            m_accumulator = 0.0;
        }
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
}
