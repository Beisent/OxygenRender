
#include "OxygenRender/Window.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Buffer.h"
#include "OxygenRender/Shader.h"
#include <iostream>
#include <filesystem>

using namespace OxyRender;

int main()
{

    Renderer renderer;
    Window window(800, 600, "OxygenRender");

    Shader program("my_shader", "./bin/shaders/vertex.vert", "./bin/shaders/fragment.frag");

    float vertices[] = {
        -0.8f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.4f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    float vertices2[] = {
        0.2f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.8f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.2f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f};

    uint32_t indices[] = {
        0, 1, 2};

    uint32_t indices2[] = {
        0, 1, 2,
        2, 3, 0};

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

    VertexLayout layout2;
    layout2.addAttribute("aPos", 0, VertexAttribType::Float3);
    layout2.addAttribute("aColor", 1, VertexAttribType::Float3);

    VertexArray vao2;
    Buffer vbo2(BufferType::Vertex, BufferUsage::StaticDraw);
    vbo2.setData(vertices2, sizeof(vertices2));

    Buffer ebo2(BufferType::Index, BufferUsage::StaticDraw);
    ebo2.setData(indices2, sizeof(indices2));

    vao2.setVertexBuffer(vbo2, layout2);
    vao2.setIndexBuffer(ebo2);

    while (!window.shouldClose())
    {
        renderer.clear();

        program.use();

        vao.bind();
        glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(sizeof(indices) / sizeof(uint32_t)), GL_UNSIGNED_INT, 0);
        vao2.bind();
        glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(sizeof(indices2) / sizeof(uint32_t)), GL_UNSIGNED_INT, 0);

        window.swapBuffers();
    }

    return 0;
}
