
#include "OxygenRender/Window.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Buffer.h"
#include "OxygenRender/OpenGLBuffers.h"
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
    Renderer renderer;
    Window window(800, 600, "OxygenRender");

    float triVertices[] = {

        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

    OxyRender::VertexLayout triLayout;
    triLayout.addAttribute("aPos", 0, OxyRender::VertexAttribType::Float3);
    triLayout.addAttribute("aColor", 1, OxyRender::VertexAttribType::Float3);

    auto triVBO = std::make_shared<GLVertexBuffer>(triVertices, sizeof(triVertices), triLayout);
    auto triVAO = std::make_shared<GLVertexArray>();
    triVAO->addVertexBuffer(triVBO);

    float sqVertices[] = {
        -0.75f, 0.75f, 0.0f, 1.0f, 1.0f, 0.0f,
        -0.25f, 0.75f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.75f, 0.25f, 0.0f, 0.5f, 0.5f, 0.5f};

    uint32_t sqIndices[] = {
        0, 1, 2,
        2, 3, 0};

    OxyRender::VertexLayout sqLayout;
    sqLayout.addAttribute("aPos", 0, OxyRender::VertexAttribType::Float3);
    sqLayout.addAttribute("aColor", 1, OxyRender::VertexAttribType::Float3);

    auto sqVAO = std::make_shared<GLVertexArray>();
    auto sqVBO = std::make_shared<GLVertexBuffer>(sqVertices, sizeof(sqVertices), sqLayout);
    auto sqEBO = std::make_shared<GLIndexBuffer>(sqIndices, 6);
    sqVAO->addVertexBuffer(sqVBO);
    sqVAO->setIndexBuffer(sqEBO);

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

    while (!window.shouldClose())
    {
        renderer.clear();
        glUseProgram(program);
        triVAO->bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        sqVAO->bind();
        auto ib = sqVAO->getIndexBuffer();
        if (ib)
        {
            glDrawElements(GL_TRIANGLES, (GLsizei)ib->getCount(), GL_UNSIGNED_INT, nullptr);
        }
        window.swapBuffers();
    }
    return 0;
}
