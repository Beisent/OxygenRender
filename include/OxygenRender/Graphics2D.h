#pragma once
#include "OxygenRender/Window.h"
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Buffer.h"
#include "OxygenRender/Camera.h"
#include "OxygenRender/Texture.h"
#include <vector>
#include <glm/glm.hpp>
#include <cmath>
#include <functional>

namespace OxyRender
{
    // 2D 绘图类
    class Graphics2D
    {
    public:
        Graphics2D(Window &window, Renderer &renderer);

        Camera &getCamera();
        void clear();
        void setClearColor(const OxyColor &color);
        void begin();

        void drawRect(float x, float y, float width, float height, OxyColor color = {1.0f, 0.0f, 0.0f, 1.0f});
        void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, OxyColor color = {1.0f, 0.0f, 0.0f, 1.0f});
       
        void drawLine(float x1, float y1, float x2, float y2, OxyColor color = {1.0f, 0.0f, 0.0f, 1.0f}, float thickness = 1.0f);
        void drawLines(const std::vector<glm::vec2> &points, OxyColor color = {1.0f, 0.0f, 0.0f, 1.0f}, float thickness = 1.0f);

        void drawCircle(float cx, float cy, float radius, OxyColor color = {1.0f, 0.0f, 0.0f, 1.0f}, int segments = 36);
        void drawCircleOutline(float cx, float cy, float radius, OxyColor color = {1.0f, 0.0f, 0.0f, 1.0f}, int segments = 36, float thickness = 1.0f);
        
        void drawEllipse(float cx, float cy, float radiusX, float radiusY,
                         OxyColor color = {1.0f, 0.0f, 0.0f, 1.0f}, int segments = 36);
        void drawEllipseOutline(float cx, float cy, float radiusX, float radiusY,
                                OxyColor color = {1.0f, 0.0f, 0.0f, 1.0f}, int segments = 36, float thickness = 1.0f);

        void drawPoints(const std::vector<glm::vec2> &points, float size, const OxyColor &color);

        void drawPolygon(const std::vector<glm::vec2> &points, OxyColor color = {1.0f, 0.0f, 0.0f, 1.0f});
        void drawPolygonOutline(const std::vector<glm::vec2> &points, OxyColor color = {1.0f, 0.0f, 0.0f, 1.0f}, float thickness = 1.0f);

        void drawArrow(float x1, float y1, float x2, float y2,
                       OxyColor color = {1.0f, 1.0f, 1.0f, 1.0f}, float thickness = 1.0, float headLength = 15.0f, float headWidth = 15.0f);

        void drawAxis(OxyColor axisColor = {1.0f, 0.0f, 0.0f, 1.0f},
                      OxyColor gridColor = {0.8f, 0.8f, 0.8f, 1.0f},
                      float thickness = 1.0f,
                      float gridSpacing = 50.0f,
                      bool drawGrid = true);

        void drawBezier(float x0, float y0,
                        float cx, float cy,
                        float x1, float y1,
                        OxyColor color,
                        float thickness = 1.0f,
                        int segments = 32);

        void drawBezier(float x0, float y0,
                        float c1x, float c1y,
                        float c2x, float c2y,
                        float x1, float y1,
                        OxyColor color,
                        float thickness = 1.0f,
                        int segments = 32);
        void drawFunction(const float &xStart, const float &xEnd,
                          const std::function<float(float)> &func,
                          const OxyColor &color,
                          const float &dx = 0.01f,
                          const float &thickness = 1.0f);

        void drawRect(float x, float y, float width, float height, const Texture2D &texture,
                      OxyColor tintColor = {1.0f, 1.0f, 1.0f, 1.0f});
        void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3,
                          const Texture2D &texture, OxyColor tintColor = {1.0f, 1.0f, 1.0f, 1.0f});
        void drawPolygon(const std::vector<glm::vec2> &points, const Texture2D &texture,
                         OxyColor tintColor = {1.0f, 1.0f, 1.0f, 1.0f});
        void drawCircle(float cx, float cy, float radius, const Texture2D &texture,
                        OxyColor tintColor = {1.0f, 1.0f, 1.0f, 1.0f}, int segments = 36);
        void drawEllipse(float cx, float cy, float radiusX, float radiusY, const Texture2D &texture,
                         OxyColor tintColor = {1.0f, 1.0f, 1.0f, 1.0f}, int segments = 36);

        // 设置当前纹理
        void setTexture(const Texture2D *texture);
        void clearTexture();

        void flush();

        // void beginMask(const std::vector<glm::vec2> &maskPolygon);
        // void endMask();

    private:
        // 顶点结构体
        struct Vertex
        {
            glm::vec3 pos;
            OxyColor color;
            glm::vec2 texCoord;
        };
        // 线段批次
        struct LineBatch
        {
            float thickness;
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            size_t indexCount = 0;
        };
        // 点批次
        struct PointBatch
        {
            float size;
            OxyColor color;
            std::vector<Vertex> vertices;
        };

        // 纹理批次
        struct TextureBatch
        {
            const Texture2D *texture;
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            size_t indexCount = 0;
        };

        Window &m_window;
        Renderer &m_renderer;
        Camera m_camera;
        Shader m_shader;
        Shader m_textureShader;

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

        // 纹理批次
        std::vector<TextureBatch> m_textureBatches;

        // 当前纹理
        const Texture2D *m_currentTexture = nullptr;

        // 辅助方法
        void addTextureVertex(float x, float y, float u, float v, OxyColor color);
        void flushTextureBatches();

        static const char* m_vertexShaderSrc;
        static const char* m_fragmentShaderSrc;
        static const char* m_textureVertexShaderSrc;
        static const char* m_textureFragmentShaderSrc;
    };
}
