#include "OxygenRender/Window.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Buffer.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Texture.h"
#include "OxygenRender/EventSystem.h"
#include "OxygenRender/Camera.h"
#include "OxygenRender/Model.h"
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

    Shader modelProgram("model_shader", "shaders/model_vertex.vert", "shaders/model_fragment.frag");

    // Shader simpleProgram("simple_shader", "shaders/vertex.vert", "shaders/fragment.frag");

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    bool mouseCaptured = false;
    float lastX = 0, lastY = 0;
    bool firstMouse = true;
    window.setCursorMode(GLFW_CURSOR_DISABLED);
    mouseCaptured = true;

    // // // 创建纹理
    // // Texture2D texture("resource/container.jpg");
    // Texture2D texture2("resource/awesomeface.png");

    // 加载模型
    Model backpackModel("resource/objects/backpack/backpack.obj");

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
                else if (key.key == GLFW_KEY_LEFT_ALT)
                {
                    window.setCursorMode(GLFW_CURSOR_NORMAL);
                    mouseCaptured = false;
                }
                break;
            }
            case EventType::KeyReleased:
            {
                auto key = std::get<KeyEvent>(e.data);
                if (key.key == GLFW_KEY_LEFT_ALT)
                {
                    window.setCursorMode(GLFW_CURSOR_DISABLED);
                    mouseCaptured = true;
                }
                break;
            }
            case EventType::MouseScrolled:
            {
                auto scroll = std::get<MouseScrollEvent>(e.data);
                camera.ProcessMouseScroll(scroll.yoffset);
                break;
            }
            case EventType::MouseMoved:
                if (mouseCaptured)
                {
                    auto mouse = std::get<MouseMoveEvent>(e.data);

                    if (firstMouse)
                    {
                        lastX = mouse.position.x;
                        lastY = mouse.position.y;
                        firstMouse = false;
                        break;
                    }

                    float xoffset = mouse.position.x - lastX;
                    float yoffset = lastY - mouse.position.y;

                    lastX = mouse.position.x;
                    lastY = mouse.position.y;

                    camera.ProcessMouseMovement(xoffset, yoffset);
                }
            default:
                break;
            }
        }

        // 处理键盘输入
        if (EventSystem::isKeyDown(GLFW_KEY_W))
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (EventSystem::isKeyDown(GLFW_KEY_S))
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (EventSystem::isKeyDown(GLFW_KEY_A))
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (EventSystem::isKeyDown(GLFW_KEY_D))
            camera.ProcessKeyboard(RIGHT, deltaTime);
        if (EventSystem::isKeyDown(GLFW_KEY_SPACE))
            camera.ProcessKeyboard(UP, deltaTime);
        if (EventSystem::isKeyDown(GLFW_KEY_LEFT_SHIFT))
            camera.ProcessKeyboard(DOWN, deltaTime);

        // 计算视图和投影矩阵
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

        renderer.clear();

        // 渲染模型
        modelProgram.use();
        modelProgram.setUniformData("view", glm::value_ptr(view), sizeof(view));
        modelProgram.setUniformData("projection", glm::value_ptr(projection), sizeof(projection));

        // 设置模型位置
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f)); 
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));      
        modelProgram.setUniformData("model", glm::value_ptr(model), sizeof(model));

        // 渲染模型
        backpackModel.Draw(modelProgram);

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}