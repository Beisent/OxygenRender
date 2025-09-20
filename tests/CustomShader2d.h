#pragma once
#include "OxygenRender/OxygenRender.h"

namespace OxyRender
{
    class CustomShader2d
    {
    public:
        static void execute()
        {
            Window window(800, 600, "OxygenRender");
            Renderer renderer(window);
            Graphics2D graphics2D(window, renderer);

            // 保存鼠标屏幕坐标
            glm::vec2 mousePosScreen(0.0f);

            // Shader 源码
            const char *vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec4 vColor;
out vec2 vLocalPos;
out vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vColor = aColor;
    vLocalPos = aPos.xy;
    vTexCoord = aTexCoord;
}
)";

            const char *fragmentShaderSrc = R"(
#version 330 core
in vec4 vColor;
in vec2 vLocalPos;
in vec2 vTexCoord;

out vec4 FragColor;

uniform vec2 uMouse;
uniform float uTime;
uniform sampler2D uTexture;
uniform bool uUseTexture;

void main()
{
    vec4 baseColor = vColor;

    // 红光中心到像素距离
    float dist = length(vLocalPos - uMouse);

    // 脉动红光
    float pulse = 0.5 + 0.5 * sin(uTime * 5.0 - dist * 0.05);
    vec3 redGlow = vec3(1.0, 0.2, 0.2) * (1.0 - smoothstep(0.0, 200.0, dist)) * pulse;
    baseColor.rgb += redGlow;

    // 波纹效果
    float angle = atan(vLocalPos.y - uMouse.y, vLocalPos.x - uMouse.x);
    float wave = sin(dist * 0.2 - uTime * 4.0 + angle * 5.0);
    baseColor.rgb *= 0.7 + 0.3 * wave;

    // 边缘发光
    float edge = smoothstep(150.0, 200.0, dist);
    baseColor.rgb += vec3(1.0, 0.5, 0.5) * (1.0 - edge) * 0.4;

    // 可选纹理混合
    if(uUseTexture)
    {
        vec4 texColor = texture(uTexture, vTexCoord);
        baseColor *= texColor;
    }

    FragColor = baseColor;
}
)";

            Shader specialShader("redGlowWorld", vertexShaderSrc, fragmentShaderSrc);
            graphics2D.setShader(&specialShader);
            graphics2D.setClearColor({0, 0, 0, 1}); // 黑色背景更炫

            auto &camera = graphics2D.getCamera();
            camera.setMovementSpeed(100.0f);

            EventSystem &eventSystem = EventSystem::getInstance();

            // 键盘事件
            eventSystem.registerCallback(EventType::KeyPressed,
                                         [&window](const Event &e)
                                         {
                                             const KeyEvent &keyEvent = std::get<KeyEvent>(e.data);
                                             if (keyEvent.key == KeyCode::Escape)
                                                 window.shutdown();
                                         });

            // 鼠标移动事件（屏幕坐标）
            eventSystem.registerCallback(EventType::MouseMoved,
                                         [&mousePosScreen](const Event &e)
                                         {
                                             const MouseMoveEvent &moveEvent = std::get<MouseMoveEvent>(e.data);
                                             mousePosScreen.x = moveEvent.positionX;
                                             mousePosScreen.y = moveEvent.positionY;
                                         });

            auto &timer = Timer::getInstance();
            timer.setTargetFPS(60);

            while (!window.shouldClose())
            {
                timer.update(window);
                double dt = timer.deltaTime();

                eventSystem.handleEvent();

                // 摄像机控制
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

         
                float ndcX = (mousePosScreen.x / window.getWidth()) * 2.0f - 1.0f;
                float ndcY = 1.0f - (mousePosScreen.y / window.getHeight()) * 2.0f;
                
                glm::mat4 proj = camera.getOrthoProjectionMatrix2D(window.getWidth(),window.getHeight());
                glm::mat4 view = camera.getOrthoViewMatrix2D();
                glm::vec4 clipPos(ndcX, ndcY, 0.0f, 1.0f);
                glm::vec4 worldPos = glm::inverse(proj * view) * clipPos;
                glm::vec2 mouseWorldPos(worldPos.x, worldPos.y);

                float time = (float)Timer::getInstance().totalTime();
                specialShader.use();
                specialShader.setUniformData("uMouse", &mouseWorldPos, sizeof(glm::vec2));
                specialShader.setUniformData("uTime", &time, sizeof(float));

                int useTexture = 0;
                specialShader.setUniformData("uUseTexture", &useTexture, sizeof(int));

                // 绘制示例图形
                graphics2D.drawPolygon({{-100, -100}, {-100, 100}, {100, 100}, {100, -100}}, {0.2f, 0.2f, 0.2f, 0.5f});
                graphics2D.drawCircle(-200, 150, 50, {0.8f, 0.1f, 0.1f, 0.5f});
                graphics2D.drawEllipse(150, 150, 80, 50, {0.5f, 0.0f, 0.5f, 0.5f});
                graphics2D.drawPolygonOutline({{-200, -200}, {-150, -100}, {-100, -200}}, {1, 0, 0, 1}, 2);

                graphics2D.flush();
                window.swapBuffers();
                window.pollEvents();
            }
        }
    };
}
