#include "OxygenRender/Time.h"

namespace OxyRender
{
    double Time::m_lastFrame = 0.0;
    double Time::m_deltaTime = 0.0;

    void Time::update(Window &window)
    {
        double currentFrame = window.getTime();
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;
    }

    double Time::deltaTime()
    {
        return m_deltaTime;
    }

    double Time::now(Window &window)
    {
        return window.getTime();
    }
}
