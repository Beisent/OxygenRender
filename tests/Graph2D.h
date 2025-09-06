#pragma once
#include "OxygenRender/Window.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Graphics2D.h"
#include "OxygenRender/EventSystem.h"
#include "OxygenRender/Timer.h"

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

            EventSystem &eventSystem = eventSystem.getInstance();
            eventSystem.registerCallback(EventType::KeyPressed,
                                         [&window](const Event &e)
                                         {
            const KeyEvent& keyEvent = std::get<KeyEvent>(e.data);
            if (keyEvent.key == KeyCode::Escape)
            {
                window.shutdown();
            } });

            eventSystem.registerCallback(EventType::MouseScrolled,
                                         [&camera](const Event &e)
                                         {
            const MouseScrollEvent& scrollEvent = std::get<MouseScrollEvent>(e.data);
            camera.processMouseScroll(scrollEvent.yoffset); });

            while (!window.shouldClose())
            {
                Timer::getInstance().update(window);
                double dt = Timer::getInstance().deltaTime();

                eventSystem.handleEvent();

                if (eventSystem.isKeyDown(KeyCode::A))
                    camera.processKeyboard(CameraMovement::LEFT, dt * camera.getZoom());
                if (eventSystem.isKeyDown(KeyCode::D))
                    camera.processKeyboard(CameraMovement::RIGHT, dt * camera.getZoom());
                if (eventSystem.isKeyDown(KeyCode::W))
                    camera.processKeyboard(CameraMovement::UP, dt * camera.getZoom());
                if (eventSystem.isKeyDown(KeyCode::S))
                    camera.processKeyboard(CameraMovement::DOWN, dt * camera.getZoom());

                graphics2D.clear();
                graphics2D.begin();

                graphics2D.drawAxis({1, 0, 0, 1}, {0.8f, 0.8f, 0.8f, 1},
                                    2.0f, 50.0f, true);
                graphics2D.flush();

                graphics2D.begin();
                graphics2D.drawCircle(-350, 250, 50, {1, 0, 0, 0.5f});
                graphics2D.drawCircleOutline(-250, 250, 50, {0, 1, 0, 1}, 64, 2);

                graphics2D.drawEllipse(100, 200, 80, 50, {0.5f, 0, 0.5f, 0.5f});
                graphics2D.drawEllipseOutline(250, 200, 80, 50, {0, 0.5f, 0.5f, 1}, 64, 2);

                std::vector<glm::vec2> pts = {
                    {0.0f, 0.0f}, {100.0f, 50.0f}, {-80.0f, 30.0f}};
                graphics2D.drawPoints(pts, 8.0f, {1, 0.2f, 0.2f, 1});
                graphics2D.drawPolygon({{-200, 200}, {-150, 300}, {-100, 200}}, {0, 0, 1, 1});
                graphics2D.drawPolygonOutline({{-100, 200}, {-50, 300}, {0, 200}}, {1, 1, 0, 1}, 2);

                graphics2D.drawArrow(100, 300, 200, 200, {1, 0, 0, 1}, 2.0f, 10.0f, 10.0f);

                graphics2D.drawBezier(10, 10, 150, 10, 200, 200, {1, 0, 0, 1}, 2.0f, 48);
                graphics2D.drawBezier(10, 10, 100, 200, 200, -50, 300, 100, {0, 1, 0, 1}, 2.0f, 64);

                auto func = [](float x)
                { return 0.01f * x * x; };
                graphics2D.drawFunction(-300, 300, [](float x)
                                        { return 100*sin(200 / x); }, {0, 0, 1, 1}, 0.1f, 2.0f);

                graphics2D.flush();
                // graphics2D.begin();

                // int gridSize = 150;
                // float spacing = 10.0f;
                // float radius = 4.0f;

                // for (int i = 0; i < gridSize; ++i)
                // {
                //     for (int j = 0; j < gridSize; ++j)
                //     {
                //         float x = i * spacing - (gridSize * spacing) / 2.0f;
                //         float y = j * spacing - (gridSize * spacing) / 2.0f;
                //         graphics2D.drawCircleOutline(x, y, radius, {0.5f, 0.2f, 0.8f, 1.0f});
                //     }
                // }

                // int gridSize = 500;
                // float spacing = 10.0f;
                // float width = 20.0f;
                // float height = 20.0f;

                // for (int i = 0; i < gridSize; ++i)
                // {
                //     for (int j = 0; j < gridSize; ++j)
                //     {
                //         float x = i * spacing - (gridSize * spacing) / 2.0f;
                //         float y = j * spacing - (gridSize * spacing) / 2.0f;
                //         if (i % 2 != j % 2)
                //         {
                //             graphics2D.drawRect(x, y, width, height, {0.2f, 0.6f, 0.9f, 1.0f});
                //         }
                //         else
                //         {
                //             graphics2D.drawRect(x, y, width, height, {0.9f, 0.2f, 0.2f, 1.0f});
                //         }
                //     }
                // }
                graphics2D.flush();

                window.swapBuffers();
                window.pollEvents();
            }
        }
    };
}