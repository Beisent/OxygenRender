#pragma once
#include "OxygenRender/Window.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Buffer.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Texture.h"
#include "OxygenRender/EventSystem.h"
#include "OxygenRender/Camera.h"
#include "OxygenRender/Model.h"
#include "OxygenRender/Time.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>

namespace OxyRender
{
    class Backpack
    {
    public:
        static void execute()
        {
            Window window(800, 600, "OxygenRender");
            Renderer renderer(window);

            Shader modelProgram("model_shader", "shaders/model_vertex.vert", "shaders/model_fragment.frag");

            Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

            window.setCursorMode(CursorMode::Disabled);
            bool mouseCaptured = true;

            Model backpackModel(renderer, "resource/objects/backpack/backpack.obj");

            while (!window.shouldClose())
            {
                Time::update(window);
                double dt = Time::deltaTime();

                Event e;
                while (EventSystem::pollEvent(e))
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
                        else if (key.key == KeyCode::LeftAlt)
                        {
                            window.setCursorMode(CursorMode::Normal);
                            mouseCaptured = false;
                        }
                        break;
                    }
                    case EventType::KeyReleased:
                    {
                        auto key = std::get<KeyEvent>(e.data);
                        if (key.key == KeyCode::LeftAlt)
                        {
                            window.setCursorMode(CursorMode::Disabled);
                            mouseCaptured = true;
                        }
                        break;
                    }
                    case EventType::MouseScrolled:
                    {
                        auto scroll = std::get<MouseScrollEvent>(e.data);
                        camera.processMouseScroll(scroll.yoffset);
                        break;
                    }
                    case EventType::MouseMoved:
                        if (mouseCaptured)
                        {
                            auto mouse = std::get<MouseMoveEvent>(e.data);
                            auto offset = EventSystem::handleMouseMoved(mouse);
                            camera.processMouseMovement(offset.x, offset.y);
                        }
                        break;
                    default:
                        break;
                    }
                }

                if (EventSystem::isKeyDown(KeyCode::W))
                    camera.processKeyboard(CameraMovement::FORWARD, dt);
                if (EventSystem::isKeyDown(KeyCode::S))
                    camera.processKeyboard(CameraMovement::BACKWARD, dt);
                if (EventSystem::isKeyDown(KeyCode::A))
                    camera.processKeyboard(CameraMovement::LEFT, dt);
                if (EventSystem::isKeyDown(KeyCode::D))
                    camera.processKeyboard(CameraMovement::RIGHT, dt);
                if (EventSystem::isKeyDown(KeyCode::Space))
                    camera.processKeyboard(CameraMovement::UP, dt);
                if (EventSystem::isKeyDown(KeyCode::LeftShift))
                    camera.processKeyboard(CameraMovement::DOWN, dt);

                glm::mat4 view = camera.getViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), 800.0f / 600.0f, 0.1f, 100.0f);

                renderer.clear();

                // 添加光照参数
                glm::vec3 lightPos = camera.getPosition();
                glm::vec3 lightAmbient(0.1f, 0.1f, 0.1f);
                glm::vec3 lightDiffuse(0.8f, 0.8f, 0.8f);
                glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

                modelProgram.use();
                modelProgram.setUniformData("light.position", glm::value_ptr(lightPos), sizeof(lightPos));
                modelProgram.setUniformData("light.ambient", glm::value_ptr(lightAmbient), sizeof(lightAmbient));
                modelProgram.setUniformData("light.diffuse", glm::value_ptr(lightDiffuse), sizeof(lightDiffuse));
                modelProgram.setUniformData("light.specular", glm::value_ptr(lightSpecular), sizeof(lightSpecular));
                modelProgram.setUniformData("viewPos", glm::value_ptr(camera.getPosition()), sizeof(glm::vec3));

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
                model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
                modelProgram.setUniformData("view", glm::value_ptr(view), sizeof(view));
                modelProgram.setUniformData("projection", glm::value_ptr(projection), sizeof(projection));
                modelProgram.setUniformData("model", glm::value_ptr(model), sizeof(model));
                renderer.setPolygonMode(RenderPolygonMode::Fill, true);
                backpackModel.Draw(modelProgram);

                window.swapBuffers();
                window.pollEvents();
            }
        }
    };

}