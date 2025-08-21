#pragma once
#include "OxygenRender/Window.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Graphics2D.h"

namespace OxyRender
{
    class Graph2D
    {
    public:
        static void execute()
        {
            Window window(800, 600, "OxygenRender");
            Renderer renderer(window);

            Graphics2D graphics2D(window, renderer);
            while (!window.shouldClose())
            {
                renderer.clear();
                graphics2D.begin();

                graphics2D.drawCircle(100, 100, 50, {1, 0, 0, 1});
                graphics2D.drawCircleOutline(200, 100, 50, {0, 1, 0, 1}, 64, 2);
                graphics2D.drawPolygon({{300, 100}, {350, 150}, {320, 180}}, {0, 0, 1, 1});
                graphics2D.drawPolygonOutline({{0, 0}, {100, 0}, {50, 100}}, {1, 1, 0, 1}, 2);

                graphics2D.drawArcAA(0, 0, 100, 0.0f, glm::radians(270.0f), {1, 0, 0, 1}, 5.0f, 128);

                graphics2D.flush();
                window.swapBuffers();
                window.pollEvents();
            }
        }
    };
}