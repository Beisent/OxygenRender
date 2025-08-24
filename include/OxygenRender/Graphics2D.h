#pragma once
#include "OxygenRender/Window.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Buffer.h"
#include "OxygenRender/Camera.h"
#include <vector>
#include <glm/glm.hpp>
#include <cmath>

namespace OxyRender
{
    class Graphics2D
    {
    public:
        Graphics2D(Window &window, Renderer &renderer);

        Camera &getCamera();
        void clear();
        void setClearColor(const OxyColor &color);
        void begin();

        void drawRect(float x, float y, float width, float height, OxyColor color = {1.0f, 1.0f, 1.0f, 1.0f});
        void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, OxyColor color = {1.0f, 1.0f, 1.0f, 1.0f});
        void drawLine(float x1, float y1, float x2, float y2, OxyColor color = {1.0f, 1.0f, 1.0f, 1.0f}, float thickness = 1.0f);

        void drawCircle(float cx, float cy, float radius, OxyColor color = {1.0f, 1.0f, 1.0f, 1.0f}, int segments = 36);
        void drawCircleOutline(float cx, float cy, float radius, OxyColor color = {1.0f, 1.0f, 1.0f, 1.0f}, int segments = 36, float thickness = 1.0f);
        void drawPolygon(const std::vector<glm::vec2> &points, OxyColor color = {1.0f, 1.0f, 1.0f, 1.0f});
        void drawPolygonOutline(const std::vector<glm::vec2> &points, OxyColor color = {1.0f, 1.0f, 1.0f, 1.0f}, float thickness = 1.0f);

        void drawArcAA(float cx, float cy, float radius, float startAngle, float endAngle, OxyColor color = {1.0f, 1.0f, 1.0f, 1.0f}, float thickness = 2.0f, int segments = 128);
        void drawArrow(float x1, float y1, float x2, float y2,
                       OxyColor color = {1.0f, 1.0f, 1.0f, 1.0f}, float thickness = 1.0, float headLength = 20.0f, float headWidth = 15.0f);
        void flush();

    private:
        struct Vertex
        {
            glm::vec3 pos;
            OxyColor color;
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
