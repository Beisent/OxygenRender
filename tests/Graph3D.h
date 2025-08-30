#pragma once
#include "OxygenRender/Window.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Graphics3D.h"
#include "OxygenRender/EventSystem.h"
#include "OxygenRender/Timer.h"

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

            window.setCursorMode(CursorMode::Disabled);
            bool mouseCaptured = true;

            while (!window.shouldClose())
            {
                Timer::getInstance().update(window);
                double dt = Timer::getInstance().deltaTime();

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
