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

                graphics2D.drawRect(0, 0, 100, 100, glm::vec4(1, 0, 0, 1));
                graphics2D.drawRect(0, -110, 100, 100, glm::vec4(0, 0, 1, 1));
                graphics2D.drawRect(0, 110, 100, 100, glm::vec4(1, 1, 0, 1));
                graphics2D.drawRect(-110, 0, 100, 100, glm::vec4(0, 1, 0, 1));

                graphics2D.flush();
                window.swapBuffers();
                window.pollEvents();
            }
        }
    };
}