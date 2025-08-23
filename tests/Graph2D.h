#pragma once
#include "OxygenRender/Window.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Graphics2D.h"
#include "OxygenRender/EventSystem.h"

namespace OxyRender
{
    class Graph2D
    {
    public:
        static void execute()
        {
            Window window(800, 600, "OxygenRender");
            Renderer renderer(window);

            Graphics2D graphics2D(window, renderer);
            auto &camera = graphics2D.getCamera();
            camera.setMovementSpeed(100.0f);

            float deltaTime = 0.0f;
            float lastFrame = 0.0f;
            
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
                if (EventSystem::isKeyDown(KeyCode::A))
                    camera.processKeyboard(CameraMovement::LEFT, deltaTime);
                if (EventSystem::isKeyDown(KeyCode::D))
                    camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
                if (EventSystem::isKeyDown(KeyCode::W))
                    camera.processKeyboard(CameraMovement::UP, deltaTime);
                if (EventSystem::isKeyDown(KeyCode::S))
                    camera.processKeyboard(CameraMovement::DOWN, deltaTime);

                renderer.clear();
                graphics2D.begin();

                graphics2D.drawCircle(100, 100, 50, {1, 0, 0, 1});
                graphics2D.drawCircleOutline(200, 100, 50, {0, 1, 0, 1}, 64, 2);
                graphics2D.drawPolygon({{300, 100}, {350, 150}, {320, 180}}, {0, 0, 1, 1});
                graphics2D.drawPolygonOutline({{0, 0}, {100, 0}, {50, 100}}, {1, 1, 0, 1}, 2);

                graphics2D.drawArcAA(0, 0, 100, 0.0f, glm::radians(270.0f), {1, 0, 0, 1}, 5.0f, 128);

                
                graphics2D.flush();
                window.swapBuffers();
                window.pollEvents();
            }
        }
    };
}