#include "OxygenRender/Renderer.h"
#include <glad/glad.h>
namespace OxyRender
{
    void OpenGLRenderer::clear()
    {
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    Renderer::Renderer()
    {
        switch (OXYG_CurrentBackend)
        {
        case RendererBackend::OpenGL:
            renderer = RendererFactory::createRenderer();
            break;
        default:
            break;
        }
    }
    void Renderer::clear()
    {
        if (renderer)
        {
            renderer->clear();
        }
    }

} // namespace OxyRender
