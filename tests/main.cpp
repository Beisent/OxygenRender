
#include "OxygenRender/Window.h"
#include "OxygenRender/Renderer.h"
#include <iostream>
using namespace OxyRender;
int main()
{
    RendererFactory factory;
    auto renderer = factory.createRenderer(RendererBackend::OpenGL);
    GLFWWindow window(800, 600, "OxygenRender");
    
    while (!window.shouldClose())
    {
        renderer->clear();

        window.swapBuffers();
    }
    return 0;
}
