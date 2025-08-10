#include "OxygenRender/Renderer.h"
#include <glad/glad.h>
namespace OxyRender
{
    void OpenGLRenderer::clear()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
} // namespace OxyRender