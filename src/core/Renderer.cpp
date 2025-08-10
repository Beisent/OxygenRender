#include "OxygenRender/Renderer.h"
#include <glad/glad.h>

void Renderer::clear()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 背景颜色
    glClear(GL_COLOR_BUFFER_BIT);   
}
