
#include "OxygenRender/Window.h"
#include "OxygenRender/Renderer.h"
#include <iostream>
using namespace OxyRender;
int main()
{
    // SetWindowBackend(WindowBackend::GLFW);
    // SetRenderBackend(RendererBackend::OpenGL);
    Renderer renderer;
    Window window(800, 600, "OxygenRender");

    while (!window.shouldClose())
    {
        renderer.clear();

        window.swapBuffers();
    }
    return 0;
}
