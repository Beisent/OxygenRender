#include "OxygenRender/Timer.h"

namespace OxyRender
{

    Timer::Timer() : m_deltaTime(0.0), m_lastFrame(0.0)
    {
    }
    Timer &Timer::getInstance()
    {

        static Timer timer;
        return timer;
    }
    void Timer::update(Window &window)
    {
        double currentFrame = window.getTime();
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;
    }

    double Timer::deltaTime()
    {
        return m_deltaTime;
    }

    double Timer::now(Window &window)
    {
        return window.getTime();
    }
}