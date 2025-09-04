#pragma once
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
namespace OxyRender
{
    class LightBox
    {
    public:
        static void execute()
        {
            Window window(800, 600, "OxygenRender");
            Renderer renderer(window);
            Shader shader("program", "../shaders/vertex.vert", "../shaders/fragment.frag");
            Shader light_shader("light", "../shaders/vertex.vert", "../shaders/light.frag");
            float vertices[] = {
                // positions          // normals           // texture coords
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

            VertexLayout layout;
            layout.addAttribute("aPos", 0, VertexAttribType::Float3);
            layout.addAttribute("aNormal", 1, VertexAttribType::Float3);
            layout.addAttribute("aTexCoord", 2, VertexAttribType::Float2);

            VertexArray vao;
            Buffer vbo(BufferType::Vertex, BufferUsage::StaticDraw);
            vbo.setData(vertices, sizeof(vertices));
            vao.setVertexBuffer(vbo, layout);

            VertexArray vao2;
            Buffer vbo2(BufferType::Vertex, BufferUsage::StaticDraw);
            vbo2.setData(vertices, sizeof(vertices));
            vao2.setVertexBuffer(vbo2, layout);

            Camera camera(glm::vec3(0.0f, 3.0f, 10.0f));
            double deltaTime = 0.0f;
            double lastFrame = 0.0f;
            float cameraAngle = 0.0f;
            float cameraRadius = 10.0f;

            EventSystem& eventSystem = eventSystem.getInstance();
            while (!window.shouldClose())
            {
                double currentFrame = window.getTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                Event e;
                while (eventSystem.pollEvent(e))
                {
                    switch (e.type)
                    {
                    case EventType::KeyPressed:
                    {
                        auto key = std::get<KeyEvent>(e.data);
                        if (key.key == KeyCode::Escape)
                        {
                            window.shutdown();
                            return;
                        }
                        break;
                    }
                    default:
                        break;
                    }
                }

                if (eventSystem.isKeyDown(KeyCode::W))
                    camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
                if (eventSystem.isKeyDown(KeyCode::S))
                    camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
                if (eventSystem.isKeyDown(KeyCode::A))
                    camera.processKeyboard(CameraMovement::LEFT, deltaTime);
                if (eventSystem.isKeyDown(KeyCode::D))
                    camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
                if (eventSystem.isKeyDown(KeyCode::Space))
                    camera.processKeyboard(CameraMovement::UP, deltaTime);
                if (eventSystem.isKeyDown(KeyCode::LeftShift))
                    camera.processKeyboard(CameraMovement::DOWN, deltaTime);

                cameraAngle += 1.0f * deltaTime;
                float radius = 5.0f;
                float lightX = sin(cameraAngle) * radius;
                float lightZ = cos(cameraAngle) * radius;
                glm::vec3 lightPos(lightX, 3.0f, lightZ);

                glm::mat4 model = glm::mat4(1.0f);
                glm::mat4 light_model = glm::translate(glm::mat4(1.0f), lightPos);
                glm::mat4 view = camera.getViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), 800.0f / 600.0f, 0.1f, 100.0f);

                glm::vec3 viewPos = camera.getPosition();
                renderer.clear();
               // renderer.setPolygonMode(RenderPolygonMode::Line, true);
                // 光源
                light_shader.use();
                light_shader.setUniformData("model", glm::value_ptr(light_model), sizeof(light_model));
                light_shader.setUniformData("view", glm::value_ptr(view), sizeof(view));
                light_shader.setUniformData("projection", glm::value_ptr(projection), sizeof(projection));
                vao2.bind();
                glDrawArrays(GL_TRIANGLES, 0, 36);

                // 物体
                shader.use();
                shader.setUniformData("model", glm::value_ptr(model), sizeof(model));
                shader.setUniformData("view", glm::value_ptr(view), sizeof(view));
                shader.setUniformData("projection", glm::value_ptr(projection), sizeof(projection));
                shader.setUniformData("objectColor", glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)), sizeof(glm::vec3));
                shader.setUniformData("lightColor", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)), sizeof(glm::vec3));
                shader.setUniformData("lightPos", glm::value_ptr(lightPos), sizeof(lightPos)); // 更新光源位置
                shader.setUniformData("viewPos", glm::value_ptr(viewPos), sizeof(viewPos));

                vao.bind();
                glDrawArrays(GL_TRIANGLES, 0, 36);

                window.swapBuffers();
                window.pollEvents();
            }
        }
    };
}