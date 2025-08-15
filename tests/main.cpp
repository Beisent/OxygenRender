#include "OxygenRender/Window.h" 
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Buffer.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Texture.h"
#include <iostream>
#include <cmath>

using namespace OxyRender;

int main()
{
    Renderer renderer;
    Window window(800, 600, "OxygenRender");

    Shader program("my_shader", "shaders/vertex.vert", "shaders/fragment.frag");

    // 三角形顶点: position + color
    float vertices[] = {
        -0.8f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.4f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.6f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    uint32_t indices[] = { 0, 1, 2 };

    VertexLayout layout;
    layout.addAttribute("aPos", 0, VertexAttribType::Float3);
    layout.addAttribute("aColor", 1, VertexAttribType::Float3);

    VertexArray vao;
    Buffer vbo(BufferType::Vertex, BufferUsage::StaticDraw);
    vbo.setData(vertices, sizeof(vertices));

    Buffer ebo(BufferType::Index, BufferUsage::StaticDraw);
    ebo.setData(indices, sizeof(indices));

    vao.setVertexBuffer(vbo, layout);
    vao.setIndexBuffer(ebo);

    // 正方形顶点: position + color + texcoord
    float vertices2[] = {
        0.2f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.8f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.8f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.2f,  0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f
    };

    uint32_t indices2[] = { 0, 1, 2, 2, 3, 0 };

    VertexLayout layout2;
    layout2.addAttribute("aPos", 0, VertexAttribType::Float3);
    layout2.addAttribute("aColor", 1, VertexAttribType::Float3);
    layout2.addAttribute("aTexCoord", 2, VertexAttribType::Float2);

    VertexArray vao2;
    Buffer vbo2(BufferType::Vertex, BufferUsage::StaticDraw);
    vbo2.setData(vertices2, sizeof(vertices2));

    Buffer ebo2(BufferType::Index, BufferUsage::StaticDraw);
    ebo2.setData(indices2, sizeof(indices2));

    vao2.setVertexBuffer(vbo2, layout2);
    vao2.setIndexBuffer(ebo2);

    Texture2D texture2D("resource/container.jpg");

    while (!window.shouldClose())
    {
        renderer.clear();

        program.use();

        // 三角形
        vao.bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // 正方形
        vao2.bind();
        texture2D.bind(0);
        int textureSlot = 0;
        program.setUniformData("uTexture", &textureSlot, sizeof(textureSlot));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.swapBuffers();
    }

    return 0;
}
