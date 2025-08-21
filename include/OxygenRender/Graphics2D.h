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

        void begin();
        void drawRect(float x, float y, float width, float height, glm::vec4 color = glm::vec4(1.0f));
        void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, glm::vec4 color = glm::vec4(1.0f));
        void drawLine(float x1, float y1, float x2, float y2, glm::vec4 color = glm::vec4(1.0f), float thickness = 1.0f);
        void flush();

    private:
        struct Vertex
        {
            glm::vec3 pos;
            glm::vec4 color;
        };

        struct LineBatch
        {
            float thickness;
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            size_t indexCount = 0;
        };

        Window &m_window;
        Renderer &m_renderer;
        Camera m_camera;
        Shader m_shader;

        VertexArray m_vao;
        Buffer m_vbo;
        Buffer m_ebo;

        // 三角形批次
        std::vector<Vertex> m_triVertices;
        std::vector<unsigned int> m_triIndices;
        size_t m_triIndexCount = 0;

        // 线段批次
        std::vector<LineBatch> m_lineBatches;
    };
}
