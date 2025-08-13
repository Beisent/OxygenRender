
#include "OxygenRender/Window.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Buffer.h"
#include <iostream>

using namespace OxyRender;

GLuint compileShader(GLenum type, const char *src)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        printf("Shader compile error: %s\n", log);
    }
    return shader;
}

GLuint createProgram(const char *vsSrc, const char *fsSrc)
{
    GLuint vs = compileShader(GL_VERTEX_SHADER, vsSrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSrc);
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        printf("Program link error: %s\n", log);
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}
int main()
{

    // ================= 顶点数据 =================
    float vertices[] = {
        // 三角形 (位置 + 颜色)
        -0.8f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.4f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        // 正方形 (位置 + 颜色)
        0.2f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.8f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.2f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f};

    uint32_t indices[] = {
        // 三角形
        0, 1, 2,
        // 正方形
        3, 4, 5,
        5, 6, 3};
    Renderer renderer;
    Window window(800, 600, "OxygenRender");

    const char *vsSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;
        out vec3 vColor;
        void main()
        {
            vColor = aColor;
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    const char *fsSrc = R"(
        #version 330 core
        in vec3 vColor;
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(vColor, 1.0);
        }
    )";

    GLuint program = createProgram(vsSrc, fsSrc);

    VertexArray vao;
    Buffer vbo(BufferType::Vertex, BufferUsage::StaticDraw);
    vbo.setData(vertices, sizeof(vertices));

    Buffer ebo(BufferType::Index, BufferUsage::StaticDraw);
    ebo.setData(indices, sizeof(indices));
    
    VertexLayout layout;
    layout.addAttribute("aPos", 0, VertexAttribType::Float3);
    layout.addAttribute("aColor", 1, VertexAttribType::Float3);

    vao.setVertexBuffer(vbo, layout);
    vao.setIndexBuffer(ebo);
    while (!window.shouldClose())
    {
        renderer.clear();
        glUseProgram(program);
        vao.bind();
        glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(sizeof(indices) / sizeof(uint32_t)), GL_UNSIGNED_INT, 0);

        window.swapBuffers();
    }
    return 0;
}
