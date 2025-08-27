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

            graphics2D.setClearColor({1.0f, 1.0f, 1.0f, 1.0f});
            auto &camera = graphics2D.getCamera();
            camera.setMovementSpeed(100.0f);

            double deltaTime = 0.0f;
            double lastFrame = 0.0f;

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
                        break;
                    }
                    case EventType::MouseScrolled:
                    {
                        auto scroll = std::get<MouseScrollEvent>(e.data);
                        camera.processMouseScroll(scroll.yoffset);
                        break;
                    }
                    default:
                        break;
                    }
                }
                if (EventSystem::isKeyDown(KeyCode::A))
                    camera.processKeyboard(CameraMovement::LEFT, deltaTime * camera.getZoom());
                if (EventSystem::isKeyDown(KeyCode::D))
                    camera.processKeyboard(CameraMovement::RIGHT, deltaTime * camera.getZoom());
                if (EventSystem::isKeyDown(KeyCode::W))
                    camera.processKeyboard(CameraMovement::UP, deltaTime * camera.getZoom());
                if (EventSystem::isKeyDown(KeyCode::S))
                    camera.processKeyboard(CameraMovement::DOWN, deltaTime * camera.getZoom());

                graphics2D.clear();
                graphics2D.begin();

                graphics2D.drawAxis(camera, window.getWidth(), window.getHeight(),
                                    {1, 0, 0, 1}, {0.8f, 0.8f, 0.8f, 1},
                                    2.0f, 50.0f, true);
                graphics2D.flush();

                graphics2D.begin();
                graphics2D.drawCircle(-350, 250, 50, {1, 0, 0, 1});
                graphics2D.drawCircleOutline(-250, 250, 50, {0, 1, 0, 1}, 64, 2);

                graphics2D.drawEllipse(100, 200, 80, 50, {0.5f, 0, 0.5f, 1});
                graphics2D.drawEllipseOutline(250, 200, 80, 50, {0, 0.5f, 0.5f, 1}, 64, 2);

                std::vector<glm::vec2> pts = {
                    {0.0f, 0.0f}, {100.0f, 50.0f}, {-80.0f, 30.0f}};
                graphics2D.drawPoints(pts, 8.0f, {1, 0.2f, 0.2f, 1});
                graphics2D.drawPolygon({{-200, 200}, {-150, 300}, {-100, 200}}, {0, 0, 1, 1});
                graphics2D.drawPolygonOutline({{-100, 200}, {-50, 300}, {0, 200}}, {1, 1, 0, 1}, 2);

                graphics2D.drawArrow(100, 300, 200, 200, {1, 0, 0, 1}, 2.0f, 10.0f, 10.0f);

                graphics2D.drawBezier(10, 10, 150, 10, 200, 200, {1, 0, 0, 1}, 2.0f, 48);

                graphics2D.drawBezier(10, 10, 100, 200, 200, -50, 300, 100, {0, 1, 0, 1}, 2.0f, 64);

                std::vector<glm::vec2> ctrl = {{10, 10}, {100, 200}, {200, -50}, {300, 200}};
                graphics2D.drawBezier(ctrl, {0, 0, 1, 1}, 1.5f, 64);

                graphics2D.flush();

                window.swapBuffers();
                window.pollEvents();
            }
        }
    };
}