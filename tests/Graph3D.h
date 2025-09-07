#pragma once
#include "OxygenRender/OxygenRender.h"

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

            while (!window.shouldClose())
            {
                Timer::getInstance().update(window);
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

                graphics3D.clear();
                graphics3D.begin();
                graphics3D.drawPlane({0.0f, -20.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1000.0f, 1000.0f}, {0.2f, 0.8f, 0.2f, 1.0f});
                graphics3D.drawBox({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.8f, 0.2f, 0.2f, 1.0f});
                graphics3D.drawSphere({0.0f, 1.5f, 0.0f}, 0.5f);
                graphics3D.drawBox({2.0f, 0.0f, 0.0f}, {1.0f, 2.0f, 1.0f}, {0.2f, 0.2f, 0.8f, 1.0f});

                graphics3D.drawFunction({-5.0f, 5.0f}, {-5.0f, 5.0f}, [](float x, float z)
                                               { return std::sin(std::sqrt(x * x + z * z)) + 5; }, {0.2f, 0.6f, 0.9f, 0.8f}, 0.2f, 0.2f);
                graphics3D.flush();

                window.swapBuffers();
                window.pollEvents();
            }
        }
    };
}
