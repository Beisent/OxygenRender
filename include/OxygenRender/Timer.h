#pragma once
#include "OxygenRender/Window.h"

namespace OxyRender
{
    class Timer
    {
    private:
        double m_lastFrame = 0.0;
        double m_deltaTime = 0.0;
        Timer();
        ~Timer() = default;

    public:
        Timer(const Timer &) = delete;
        Timer &operator=(const Timer &) = delete;
        static Timer &getInstance();
        void update(Window &window);

        double deltaTime();
        double now(Window &window);
    };
}
