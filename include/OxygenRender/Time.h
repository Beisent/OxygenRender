#pragma once
#include "OxygenRender/Window.h"

namespace OxyRender
{
    class Time
    {
    private:
        static double m_lastFrame;
        static double m_deltaTime;

    public:
        static void update(Window &window);

        static double deltaTime();
        static double now(Window &window);
    };
}
