#include "OxygenRender/OxygenRender.h"
using namespace OxyRender;

class Simple2D
{
public:
    static void execute()
    {
        Window window(800, 600, "Simple2D");
        Renderer renderer(window);

        Graphics2D graphics2D(window, renderer);

        while (!window.shouldClose())
        {
            graphics2D.clear();
            graphics2D.begin();

            graphics2D.drawRect(-50, 50, 100, -100, {1, 0, 0, 0.5f});

            graphics2D.flush();

            window.update();
        }
    }
};