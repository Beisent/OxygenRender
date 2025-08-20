#pragma once
#include "OxygenRender/Window.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Buffer.h"
#include "OxygenRender/Camera.h"
#include <vector>
#include <glm/glm.hpp>

namespace OxyRender
{
    class Graphics2D
    {
    public:
        Graphics2D(Window &window, Renderer &renderer);

        void begin(); // 开始批量绘制
        void drawRect(float x, float y, float width, float height, glm::vec4 color = glm::vec4(1.0f));
        void flush(); // 提交渲染

    private:
        struct Vertex
        {
            glm::vec3 pos;
            glm::vec4 color;
        };

        Window &m_window;
        Renderer &m_renderer;
        Camera m_camera;
        Shader m_shader;

        VertexArray m_vao;
        Buffer m_vbo;
        Buffer m_ebo;

        std::vector<Vertex> m_cpuVertices;
        std::vector<unsigned int> m_cpuIndices;
        size_t m_indexCount = 0;
    };
}
