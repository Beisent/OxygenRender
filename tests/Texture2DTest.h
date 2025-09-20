#pragma once
#include "OxygenRender/OxygenRender.h"

namespace OxyRender
{
    class Texture2DTest
    {
    public:
        static void execute()
        {
            Window window(800, 600, "OxygenRender - Texture2D Test");
            Renderer renderer(window);
            Graphics2D graphics2D(window, renderer);

            graphics2D.setClearColor({0.2f, 0.2f, 0.2f, 1.0f});
            auto &camera = graphics2D.getCamera();
            camera.setMovementSpeed(100.0f);

            // 事件系统
            EventSystem &eventSystem = EventSystem::getInstance();
            eventSystem.registerCallback(EventType::KeyPressed,
                                         [&window](const Event &e)
                                         {
                                             const KeyEvent &keyEvent = std::get<KeyEvent>(e.data);
                                             if (keyEvent.key == KeyCode::Escape)
                                                 window.shutdown();
                                         });

            eventSystem.registerCallback(EventType::MouseScrolled,
                                         [&camera](const Event &e)
                                         {
                                             const MouseScrollEvent &scrollEvent = std::get<MouseScrollEvent>(e.data);
                                             camera.processMouseScroll(scrollEvent.yOffset);
                                         });

            auto &res = ResourcesManager::getInstance();
            Texture2D containerTexture(res.resolve("container.jpg"));
            Texture2D faceTexture(res.resolve("awesomeface.png"));

            auto &timer = Timer::getInstance();
            timer.setTargetFPS(90); 


            while (!window.shouldClose())
            {
                timer.update(window);
                double dt = timer.deltaTime();


                eventSystem.handleEvent();

                // 相机移动
                if (eventSystem.isKeyDown(KeyCode::A))
                    camera.processKeyboard(CameraMovement::Left, dt * camera.getZoom());
                if (eventSystem.isKeyDown(KeyCode::D))
                    camera.processKeyboard(CameraMovement::Right, dt * camera.getZoom());
                if (eventSystem.isKeyDown(KeyCode::W))
                    camera.processKeyboard(CameraMovement::Up, dt * camera.getZoom());
                if (eventSystem.isKeyDown(KeyCode::S))
                    camera.processKeyboard(CameraMovement::Down, dt * camera.getZoom());

                graphics2D.clear();
                graphics2D.begin();

                // 绘制内容
                graphics2D.drawAxis({1, 0, 0, 1}, {0.8f, 0.8f, 0.8f, 1}, 2.0f, 50.0f, true);

                graphics2D.drawRect(-300, 200, 100, 100, containerTexture, {1.0f,1.0f,1.0f,1.0f});
                graphics2D.drawRect(-150, 200, 100, 100, faceTexture, {1.0f,1.0f,1.0f,1.0f});

                graphics2D.drawTriangle(100, 200, 200, 200, 150, 300, faceTexture, {1.0f,1.0f,1.0f,1.0f});

                std::vector<glm::vec2> polygonPoints = {
                    {250, 200}, {300, 250}, {250, 300}, {200, 250}};
                graphics2D.drawPolygon(polygonPoints, containerTexture, {1.0f,1.0f,1.0f,0.8f});

                graphics2D.drawCircle(100, -200, 60, faceTexture, {1.0f,1.0f,1.0f,0.9f}, 48);
                graphics2D.drawEllipse(250, -200, 80, 50, containerTexture, {1.0f,1.0f,1.0f,0.8f}, 48);

                graphics2D.drawCircle(-350, -200, 50, {1,0,0,0.5f});
                graphics2D.drawRect(-250, -250, 80, 80, {0,1,0,0.7f});
                graphics2D.drawTriangle(100, -250, 150, -200, 200, -250, {0,0,1,0.6f});

                graphics2D.drawLine(-400, -100, -300, -100, {1,1,0,1}, 3.0f);
               

                graphics2D.flush(); 
                window.swapBuffers();
                window.pollEvents();
            }
        }
    };
}
