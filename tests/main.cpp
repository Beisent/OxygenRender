#include "OxygenRender/Window.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Buffer.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Texture.h"
#include "OxygenRender/EventSystem.h"
#include "OxygenRender/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>

using namespace OxyRender;

int main()
{

    Window window(800, 600, "OxygenRender");
    Renderer renderer(window);
    Shader program("my_shader", "shaders/vertex.vert", "shaders/fragment.frag");

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    std::vector<glm::vec3> cubePositions = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    VertexLayout layout2;
    layout2.addAttribute("aPos", 0, VertexAttribType::Float3);
    layout2.addAttribute("aTexCoord", 1, VertexAttribType::Float2);

    VertexArray vao2;
    Buffer vbo2(BufferType::Vertex, BufferUsage::StaticDraw);
    vbo2.setData(vertices, sizeof(vertices));

    vao2.setVertexBuffer(vbo2, layout2);

    Texture2D texture2D("resource/container.jpg");
    Texture2D texture2D2("resource/awesomeface.png");

    while (!window.shouldClose())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Event e;
        while (EventSystem::pollEvent(e))
        {
            switch (e.type)
            {
            case EventType::KeyPressed:
            {
                auto key = std::get<KeyEvent>(e.data);
                if (key.key == GLFW_KEY_ESCAPE)
                {
                    window.shutdown();
                    return 0;
                }
                break;
            }
            default:
                break;
            }
        }

        if (OxyRender::EventSystem::isKeyDown(GLFW_KEY_W))
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (OxyRender::EventSystem::isKeyDown(GLFW_KEY_S))
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (OxyRender::EventSystem::isKeyDown(GLFW_KEY_A))
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (OxyRender::EventSystem::isKeyDown(GLFW_KEY_D))
            camera.ProcessKeyboard(RIGHT, deltaTime);
        if (OxyRender::EventSystem::isKeyDown(GLFW_KEY_SPACE))
            camera.ProcessKeyboard(UP, deltaTime);
        if (OxyRender::EventSystem::isKeyDown(GLFW_KEY_LEFT_SHIFT))
            camera.ProcessKeyboard(DOWN, deltaTime);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

        renderer.clear();
        program.use();
        program.setUniformData("view", glm::value_ptr(view), sizeof(view));
        program.setUniformData("projection", glm::value_ptr(projection), sizeof(projection));

        int textureSlot = 0;
        program.setUniformData("uTexture", &textureSlot, sizeof(textureSlot));

        vao2.bind();

        for (unsigned int i = 0; i < cubePositions.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);

            float angle = 20.0f * (i + 1);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            program.setUniformData("model", glm::value_ptr(model), sizeof(model));
            if (i % 2 == 1)
            {
                texture2D2.bind(0);
            }
            else
            {
                texture2D.bind(0);
            }

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}
