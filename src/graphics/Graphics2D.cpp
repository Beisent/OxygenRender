#include "OxygenRender/Graphics2D.h"
namespace OxyRender
{
    Graphics2D::Graphics2D(Window &window, Renderer &renderer)
        : m_window(window),
          m_renderer(renderer),
          m_camera(glm::vec3(0, 0, 10.0f)),
          m_shader("default", "shaders/graphics2d/vertex.vert", "shaders/graphics2d/fragment.frag"),
          m_vbo(BufferType::Vertex, BufferUsage::DynamicDraw),
          m_ebo(BufferType::Index, BufferUsage::DynamicDraw)
    {
        // 创建顶点布局
        VertexLayout layout;
        layout.addAttribute("aPos", 0, VertexAttribType::Float3);
        layout.addAttribute("aColor", 1, VertexAttribType::Float4);

        // 设置数据
        m_vao.setVertexBuffer(m_vbo, layout);
        m_vao.setIndexBuffer(m_ebo);

        // 初始化渲染
        m_renderer.setCapability(RenderCapability::Blend, false);
        m_renderer.setCapability(RenderCapability::DepthTest, true);

        // 初始化相机
        m_camera.setZoom(1.0);
    }
    void Graphics2D::clear()
    {
        m_renderer.clear();
    }
    void Graphics2D::setClearColor(const OxyColor &color)
    {
        m_renderer.setClearColor(color);
    }
    Camera &Graphics2D::getCamera()
    {
        return m_camera;
    }
    void Graphics2D::begin()
    {
        m_triVertices.clear();
        m_triIndices.clear();
        m_triIndexCount = 0;

        m_lineBatches.clear();
    }

    void Graphics2D::drawRect(float x, float y, float width, float height, OxyColor color)
    {
        Vertex v0 = {{x, y, 0.0f}, color};
        Vertex v1 = {{x + width, y, 0.0f}, color};
        Vertex v2 = {{x + width, y + height, 0.0f}, color};
        Vertex v3 = {{x, y + height, 0.0f}, color};

        unsigned int startIndex = (unsigned int)m_triVertices.size();
        m_triVertices.push_back(v0);
        m_triVertices.push_back(v1);
        m_triVertices.push_back(v2);
        m_triVertices.push_back(v3);

        m_triIndices.push_back(startIndex + 0);
        m_triIndices.push_back(startIndex + 1);
        m_triIndices.push_back(startIndex + 2);
        m_triIndices.push_back(startIndex + 2);
        m_triIndices.push_back(startIndex + 3);
        m_triIndices.push_back(startIndex + 0);

        m_triIndexCount += 6;
    }

    void Graphics2D::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, OxyColor color)
    {
        Vertex v1 = {{x1, y1, 0.0f}, color};
        Vertex v2 = {{x2, y2, 0.0f}, color};
        Vertex v3 = {{x3, y3, 0.0f}, color};

        unsigned int startIndex = (unsigned int)m_triVertices.size();
        m_triVertices.push_back(v1);
        m_triVertices.push_back(v2);
        m_triVertices.push_back(v3);

        m_triIndices.push_back(startIndex + 0);
        m_triIndices.push_back(startIndex + 1);
        m_triIndices.push_back(startIndex + 2);

        m_triIndexCount += 3;
    }

    void Graphics2D::drawLine(float x1, float y1, float x2, float y2, OxyColor color, float thickness)
    {

        LineBatch *batch = nullptr;
        for (auto &b : m_lineBatches)
        {
            if (fabs(b.thickness - thickness) < 0.001f)
            {
                batch = &b;
                break;
            }
        }
        if (!batch)
        {
            m_lineBatches.push_back(LineBatch{thickness});
            batch = &m_lineBatches.back();
        }

        Vertex v1 = {{x1, y1, 0.0f}, color};
        Vertex v2 = {{x2, y2, 0.0f}, color};

        unsigned int startIndex = (unsigned int)batch->vertices.size();
        batch->vertices.push_back(v1);
        batch->vertices.push_back(v2);

        batch->indices.push_back(startIndex + 0);
        batch->indices.push_back(startIndex + 1);

        batch->indexCount += 2;
    }

    void Graphics2D::drawCircle(float cx, float cy, float radius, OxyColor color, int segments)
    {
        if (segments < 3)
            segments = 3;

        unsigned int startIndex = (unsigned int)m_triVertices.size();
        // 圆心
        m_triVertices.push_back({{cx, cy, 0.0f}, color});

        for (int i = 0; i <= segments; i++)
        {
            float angle = (float)i / segments * 2.0f * 3.14159265358979323846f;
            float x = cx + cos(angle) * radius;
            float y = cy + sin(angle) * radius;
            m_triVertices.push_back({{x, y, 0.0f}, color});
        }

        for (int i = 1; i <= segments; i++)
        {
            m_triIndices.push_back(startIndex);
            m_triIndices.push_back(startIndex + i);
            m_triIndices.push_back(startIndex + i + 1);
            m_triIndexCount += 3;
        }
    }

    void Graphics2D::drawCircleOutline(float cx, float cy, float radius, OxyColor color, int segments, float thickness)
    {
        if (segments < 3)
            segments = 3;

        float prevX = cx + radius;
        float prevY = cy;
        for (int i = 1; i <= segments; i++)
        {
            float angle = (float)i / segments * 2.0f * 3.14159265358979323846f;
            float x = cx + cos(angle) * radius;
            float y = cy + sin(angle) * radius;

            drawLine(prevX, prevY, x, y, color, thickness);

            prevX = x;
            prevY = y;
        }
    }

    void Graphics2D::drawEllipse(float cx, float cy, float radiusX, float radiusY,
                                 OxyColor color, int segments)
    {
        if (segments < 3)
            segments = 3;

        unsigned int startIndex = (unsigned int)m_triVertices.size();
        m_triVertices.push_back({{cx, cy, 0.0f}, color}); // 椭圆中心

        for (int i = 0; i <= segments; i++)
        {
            float angle = (float)i / segments * 2.0f * 3.14159265358979323846f;
            float x = cx + cos(angle) * radiusX;
            float y = cy + sin(angle) * radiusY;
            m_triVertices.push_back({{x, y, 0.0f}, color});
        }

        for (int i = 1; i <= segments; i++)
        {
            m_triIndices.push_back(startIndex);
            m_triIndices.push_back(startIndex + i);
            m_triIndices.push_back(startIndex + i + 1);
            m_triIndexCount += 3;
        }
    }

    void Graphics2D::drawEllipseOutline(float cx, float cy, float radiusX, float radiusY,
                                        OxyColor color, int segments, float thickness)
    {
        if (segments < 3)
            segments = 3;

        float prevX = cx + radiusX;
        float prevY = cy;
        for (int i = 1; i <= segments; i++)
        {
            float angle = (float)i / segments * 2.0f * 3.14159265358979323846f;
            float x = cx + cos(angle) * radiusX;
            float y = cy + sin(angle) * radiusY;

            drawLine(prevX, prevY, x, y, color, thickness);

            prevX = x;
            prevY = y;
        }
    }

    void Graphics2D::drawPolygon(const std::vector<glm::vec2> &points, OxyColor color)
    {
        if (points.size() < 3)
            return;

        unsigned int startIndex = (unsigned int)m_triVertices.size();

        for (auto &p : points)
            m_triVertices.push_back({{p.x, p.y, 0.0f}, color});

        for (size_t i = 1; i + 1 < points.size(); i++)
        {
            m_triIndices.push_back(startIndex);
            m_triIndices.push_back(startIndex + i);
            m_triIndices.push_back(startIndex + i + 1);
            m_triIndexCount += 3;
        }
    }

    void Graphics2D::drawPolygonOutline(const std::vector<glm::vec2> &points, OxyColor color, float thickness)
    {
        if (points.size() < 2)
            return;

        for (size_t i = 0; i < points.size(); i++)
        {
            glm::vec2 p1 = points[i];
            glm::vec2 p2 = points[(i + 1) % points.size()];
            drawLine(p1.x, p1.y, p2.x, p2.y, color, thickness);
        }
    }

    void Graphics2D::drawArrow(float x1, float y1, float x2, float y2,
                               OxyColor color, float thickness, float headLength, float headWidth)
    {

        drawLine(x1, y1, x2, y2, color, thickness);

        glm::vec2 dir = glm::normalize(glm::vec2(x2 - x1, y2 - y1));
        glm::vec2 perp = glm::vec2(-dir.y, dir.x);

        glm::vec2 base = glm::vec2(x2, y2) - dir * headLength;

        glm::vec2 v0 = glm::vec2(x2, y2);
        glm::vec2 v1 = base + perp * (headWidth * 0.5f);
        glm::vec2 v2 = base - perp * (headWidth * 0.5f);

        drawTriangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, color);
    }

    void Graphics2D::drawAxis(const Camera &camera,
                              float windowWidth, float windowHeight,
                              OxyColor axisColor,
                              OxyColor gridColor,
                              float thickness,
                              float gridSpacing,
                              bool drawGrid)
    {
        glm::mat4 proj = camera.get2DOrthoProjectionMatrix(windowWidth, windowHeight);
        glm::mat4 view = camera.get2DOrthoViewMatrix();
        glm::mat4 vp = proj * view;
        glm::mat4 invVP = glm::inverse(vp);

        auto unproject = [&](float ndcX, float ndcY)
        {
            glm::vec4 p(ndcX, ndcY, 0.0f, 1.0f);
            p = invVP * p;
            return glm::vec2(p.x / p.w, p.y / p.w);
        };

        glm::vec2 bottomLeft = unproject(-1, -1);
        glm::vec2 topRight = unproject(1, 1);

        if (drawGrid && gridSpacing > 0.0f)
        {
            float yStart = std::floor(bottomLeft.y / gridSpacing) * gridSpacing;
            for (float y = yStart; y <= topRight.y; y += gridSpacing)
            {
                drawLine(bottomLeft.x, y, topRight.x, y, gridColor, 1.0f);
            }
            float xStart = std::floor(bottomLeft.x / gridSpacing) * gridSpacing;
            for (float x = xStart; x <= topRight.x; x += gridSpacing)
            {
                drawLine(x, bottomLeft.y, x, topRight.y, gridColor, 1.0f);
            }
        }
        drawArrow(bottomLeft.x, 0, topRight.x, 0, axisColor, thickness);
        drawArrow(0, bottomLeft.y, 0, topRight.y, axisColor, thickness);
    }

    void Graphics2D::flush()
    {
        if (m_triIndexCount == 0 && m_lineBatches.empty())
            return;

        m_renderer.setCapability(RenderCapability::Multisample, true);
        m_renderer.setCapability(RenderCapability::Blend, true);
        m_renderer.setCapability(RenderCapability::DepthTest, false);
        m_renderer.setCapability(RenderCapability::StencilTest, false);

        m_shader.use();

        // MVP变换
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = m_camera.get2DOrthoViewMatrix();
        glm::mat4 projection = m_camera.get2DOrthoProjectionMatrix(m_window.getWidth(), m_window.getHeight());

        m_shader.setUniformData("model", &model, sizeof(glm::mat4));
        m_shader.setUniformData("view", &view, sizeof(glm::mat4));
        m_shader.setUniformData("projection", &projection, sizeof(glm::mat4));

        m_vao.bind();

        for (auto &batch : m_lineBatches)
        {
            if (batch.indexCount == 0)
                continue;

            m_vbo.setData(batch.vertices.data(), batch.vertices.size() * sizeof(Vertex));
            m_ebo.setData(batch.indices.data(), batch.indices.size() * sizeof(unsigned int));
            m_renderer.drawLines(m_vao, batch.indexCount, batch.thickness);
        }

        if (m_triIndexCount > 0)
        {
            m_vbo.setData(m_triVertices.data(), m_triVertices.size() * sizeof(Vertex));
            m_ebo.setData(m_triIndices.data(), m_triIndices.size() * sizeof(unsigned int));
            m_renderer.drawTriangles(m_vao, m_triIndexCount);
        }

        m_vao.unbind();

        m_triIndexCount = 0;
        m_lineBatches.clear();
    }
    // void Graphics2D::beginMask(const std::vector<glm::vec2> &maskPolygon)
    // {
    //     bool wasDepthTestEnabled = false;

    //     m_renderer.setCapability(RenderCapability::Blend, false);
    //     m_renderer.setCapability(RenderCapability::StencilTest, true);

    //     glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    //     m_renderer.setStencilFunc(StencilFunc::Always, 1, 0xFF);
    //     m_renderer.setStencilOp(StencilOp::Replace, StencilOp::Replace, StencilOp::Replace);
    //     m_renderer.setStencilMask(0xFF);
    //     m_renderer.clearStencil();

    //     drawPolygon(maskPolygon, {0, 0, 0, 0});
    //     flush();

    //     glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    //     m_renderer.setStencilFunc(StencilFunc::Equal, 1, 0xFF);
    //     m_renderer.setStencilMask(0x00);
    // }

    // void Graphics2D::endMask()
    // {
    //     // 重置模板测试
    //     m_renderer.setCapability(RenderCapability::StencilTest, false);
    //     // 恢复颜色遮罩
    //     glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    // }

}
