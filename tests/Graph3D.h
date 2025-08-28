#pragma once
#include "OxygenRender/Window.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Graphics3D.h"
#include "OxygenRender/EventSystem.h"

namespace OxyRender
{
    class Graph3D
    {
    public:
        static void execute()
        {
            Window window(1280, 720, "OxygenRender");
            Renderer renderer(window);
            Graphics3D graphics3D(window, renderer);

            graphics3D.setClearColor({0.9f, 0.9f, 0.9f, 1.0f});

            auto &camera = graphics3D.getCamera();
            camera.setPosition({0.0f, 0.0f, 5.0f});
            camera.setMovementSpeed(3.0f);
            camera.setMouseSensitivity(0.2f);

            double deltaTime = 0.0f;
            double lastFrame = 0.0f;
            bool mouseCaptured = false;
            float lastX = 0, lastY = 0;
            bool firstMouse = true;
            window.setCursorMode(CursorMode::Disabled);
            mouseCaptured = true;

            while (!window.shouldClose())
            {
                double currentFrame = window.getTime();
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

                            camera.processMouseMovement(xoffset, yoffset);
                        }
                    default:
                        break;
                    }
                }

                if (EventSystem::isKeyDown(KeyCode::W))
                    camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
                if (EventSystem::isKeyDown(KeyCode::S))
                    camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
                if (EventSystem::isKeyDown(KeyCode::A))
                    camera.processKeyboard(CameraMovement::LEFT, deltaTime);
                if (EventSystem::isKeyDown(KeyCode::D))
                    camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
                if (EventSystem::isKeyDown(KeyCode::Space))
                    camera.processKeyboard(CameraMovement::UP, deltaTime);
                if (EventSystem::isKeyDown(KeyCode::LeftShift))
                    camera.processKeyboard(CameraMovement::DOWN, deltaTime);
                // --- Rendering ---
                graphics3D.clear();
                graphics3D.begin();
                graphics3D.drawBox({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.8f, 0.2f, 0.2f, 1.0f});
                graphics3D.drawSphere({0.0f, 1.5f, 0.0f}, 0.5f);
                graphics3D.drawBox({2.0f, 0.0f, 0.0f}, {1.0f, 2.0f, 1.0f}, {0.2f, 0.2f, 0.8f, 1.0f});
                graphics3D.flush();

                window.swapBuffers();
                window.pollEvents();
            }
        }
    };
}
