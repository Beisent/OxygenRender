#pragma once
#include "OxygenRender/Window.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Buffer.h"
#include "OxygenRender/Camera.h"
#include "OxygenRender/OxygenMathLite.h"
#include <vector>
#include <cmath>
#include <functional>

namespace OxyRender
{
    // 3D 绘图类
    class Graphics3D
    {
    public:
        Graphics3D(Window &window, Renderer &renderer);

        Camera &getCamera();
        void clear();
        void setClearColor(const OxyColor &color);
        void setShader(Shader *shader) { m_customShader = shader; }
        void begin();

        void drawTriangle(const MathLite::Vec3 &p1,
                          const MathLite::Vec3 &p2,
                          const MathLite::Vec3 &p3,
                          OxyColor color = {1, 1, 1, 1});

        void drawLine(const MathLite::Vec3 &p1,
                      const MathLite::Vec3 &p2,
                      OxyColor color = {1, 1, 1, 1},
                      float thickness = 1.0f);

        void drawPoints(const std::vector<MathLite::Vec3> &points,
                        float size,
                        const OxyColor &color);

        void drawPlane(const MathLite::Vec3 &center, const MathLite::Vec3 &normal, const MathLite::Vec2 &size, const OxyColor &color);

        void drawBox(const MathLite::Vec3 &center,
                     const MathLite::Vec3 &size,
                     const OxyColor &color = {0.8f, 0.2f, 0.2f, 1.0f});

        void drawSphere(const MathLite::Vec3 &center,
                        float radius,
                        int stacks = 16,
                        int slices = 24,
                        const OxyColor &color = {0.2f, 0.6f, 0.9f, 1.0f});
        void drawFunction(
            const MathLite::Vec2 &xValues,
            const MathLite::Vec2 &yValues,
            const std::function<float(float, float)> &func,
            const OxyColor &color = {0.2f, 0.6f, 0.9f, 1.0f},
            const float &dx = 0.1f,
            const float &dy = 0.1f);
        void drawCylinder(const MathLite::Vec3 &center,
                          float radius,
                          float height,
                          int slices,
                          const OxyColor &color,
                          bool capped);

        void flush();

    private:
        struct Vertex
        {
            MathLite::Vec3 pos;
            OxyColor color;
            MathLite::Vec3 normal;
        };

        struct LineBatch
        {
            float thickness = 1.0f;
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            size_t indexCount = 0;
        };

        struct PointBatch
        {
            float size = 1.0f;
            OxyColor color{1, 1, 1, 1};
            std::vector<Vertex> vertices;
        };

        Window &m_window;
        Renderer &m_renderer;
        Camera m_camera;
        Shader m_shader;
        Shader *m_customShader = nullptr;

        VertexArray m_vao;
        Buffer m_vbo;
        Buffer m_ebo;

        // 三角形批次
        std::vector<Vertex> m_triVertices;
        std::vector<unsigned int> m_triIndices;
        size_t m_triIndexCount = 0;

        // 线段批次
        std::vector<LineBatch> m_lineBatches;

        // 点批次
        std::vector<PointBatch> m_pointBatches;

        // 硬编码的着色器源码
        static const char *m_vertexShaderSrc;
        static const char *m_fragmentShaderSrc;
    };
}