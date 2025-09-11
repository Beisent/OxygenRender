#pragma once
#include "OxygenRender/OxygenRender.h"
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

            auto &res = ResourcesManager::getInstance();

            Shader modelProgram = Model::CreateDefaultShader();

            Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

            window.setCursorMode(CursorMode::Disabled);
            bool mouseCaptured = true;
            Model backpackModel(renderer, "../resources/objects/backpack/backpack.obj");
            EventSystem &eventSystem = eventSystem.getInstance();

            // 注册键盘按下事件
            eventSystem.registerCallback(EventType::KeyPressed, [&window, &mouseCaptured](const Event &e)
                                         {
            const KeyEvent& keyEvent = std::get<KeyEvent>(e.data);
            if (keyEvent.key == KeyCode::Escape)
            {
                window.shutdown();
            }
            else if (keyEvent.key == KeyCode::LeftAlt)
            {
                window.setCursorMode(CursorMode::Normal);
                mouseCaptured = false;
            } });

            // 注册键盘释放事件
            eventSystem.registerCallback(EventType::KeyReleased, [&window, &mouseCaptured](const Event &e)
                                         {
            const KeyEvent& keyEvent = std::get<KeyEvent>(e.data);
            if (keyEvent.key == KeyCode::LeftAlt)
            {
                window.setCursorMode(CursorMode::Disabled);
                mouseCaptured = true;
            } });

            // 注册鼠标滚动事件
            eventSystem.registerCallback(EventType::MouseScrolled, [&camera](const Event &e)
                                         {
            const MouseScrollEvent& scrollEvent = std::get<MouseScrollEvent>(e.data);
            camera.processMouseScroll(scrollEvent.yoffset); });

            // 注册鼠标移动事件
            eventSystem.registerCallback(EventType::MouseMoved, [&camera, &eventSystem, &mouseCaptured](const Event &e)
                                         {
            if (mouseCaptured)
            {
                const MouseMoveEvent& mouse = std::get<MouseMoveEvent>(e.data);
                auto offset = eventSystem.handleMouseMoved(mouse);
                camera.processMouseMovement(offset.x, offset.y);
            } });

            auto &timer = OxyRender::Timer::getInstance();
            timer.setTargetFPS(60);//锁定 60 FPS
            while (!window.shouldClose())
            {
                timer.update(window);
                double dt = Timer::getInstance().deltaTime();

                eventSystem.handleEvent();

                if (eventSystem.isKeyDown(KeyCode::W))
                    camera.processKeyboard(CameraMovement::FORWARD, dt);
                if (eventSystem.isKeyDown(KeyCode::S))
                    camera.processKeyboard(CameraMovement::BACKWARD, dt);
                if (eventSystem.isKeyDown(KeyCode::A))
                    camera.processKeyboard(CameraMovement::LEFT, dt);
                if (eventSystem.isKeyDown(KeyCode::D))
                    camera.processKeyboard(CameraMovement::RIGHT, dt);
                if (eventSystem.isKeyDown(KeyCode::Space))
                    camera.processKeyboard(CameraMovement::UP, dt);
                if (eventSystem.isKeyDown(KeyCode::LeftShift))
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