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
        VertexLayout layout;
        layout.addAttribute("aPos", 0, VertexAttribType::Float3);
        layout.addAttribute("aColor", 1, VertexAttribType::Float4);

        m_vao.setVertexBuffer(m_vbo, layout);
        m_vao.setIndexBuffer(m_ebo);
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

    void Graphics2D::drawArcAA(float cx, float cy, float radius,
                               float startAngle, float endAngle,
                               OxyColor color, float thickness, int segments)
    {
        if (segments < 8)
            segments = 8;
        if (endAngle < startAngle)
            std::swap(startAngle, endAngle);

        unsigned int startIndex = (unsigned int)m_triVertices.size();

        float rInner = radius - thickness * 0.5f;
        float rOuter = radius + thickness * 0.5f;

        for (int i = 0; i <= segments; i++)
        {
            float t = (float)i / segments;
            float angle = startAngle + t * (endAngle - startAngle);

            float cs = cos(angle);
            float sn = sin(angle);

            glm::vec3 pOuter = {cx + cs * rOuter, cy + sn * rOuter, 0.0f};
            glm::vec3 pInner = {cx + cs * rInner, cy + sn * rInner, 0.0f};

            m_triVertices.push_back({pOuter, color});
            m_triVertices.push_back({pInner, color});
        }

        for (int i = 0; i < segments; i++)
        {
            unsigned int i0 = startIndex + i * 2;
            unsigned int i1 = startIndex + i * 2 + 1;
            unsigned int i2 = startIndex + i * 2 + 2;
            unsigned int i3 = startIndex + i * 2 + 3;

            m_triIndices.push_back(i0);
            m_triIndices.push_back(i2);
            m_triIndices.push_back(i1);

            m_triIndices.push_back(i2);
            m_triIndices.push_back(i3);
            m_triIndices.push_back(i1);

            m_triIndexCount += 6;
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
    void Graphics2D::flush()
    {
        if (m_triIndexCount == 0 && m_lineBatches.empty())
            return;

        glEnable(GL_MULTISAMPLE);
        m_renderer.setCapability(RenderCapability::Blend, true);
        m_renderer.setCapability(RenderCapability::DepthTest, false);
        m_renderer.setCapability(RenderCapability::StencilTest, false);

        m_shader.use();
        m_camera.setZoom(1.0);
        // m_camera.setPosition(glm::vec3(100.0f, 100.0f, 0.0f));
        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(m_camera.getZoom(), m_camera.getZoom(), 1.0f));
        glm::mat4 view = m_camera.get2DOrthoViewMatrix();
        glm::mat4 projection = m_camera.get2DOrthoProjectionMatrix(m_window.getWidth(), m_window.getHeight());

        m_shader.setUniformData("model", &model, sizeof(glm::mat4));
        m_shader.setUniformData("view", &view, sizeof(glm::mat4));
        m_shader.setUniformData("projection", &projection, sizeof(glm::mat4));

        m_vao.bind();

        if (m_triIndexCount > 0)
        {
            m_vbo.setData(m_triVertices.data(), m_triVertices.size() * sizeof(Vertex));
            m_ebo.setData(m_triIndices.data(), m_triIndices.size() * sizeof(unsigned int));
            m_renderer.drawTriangles(m_vao, m_triIndexCount);
        }

        for (auto &batch : m_lineBatches)
        {
            if (batch.indexCount == 0)
                continue;

            m_vbo.setData(batch.vertices.data(), batch.vertices.size() * sizeof(Vertex));
            m_ebo.setData(batch.indices.data(), batch.indices.size() * sizeof(unsigned int));
            m_renderer.drawLines(m_vao, batch.indexCount, batch.thickness);
        }

        m_vao.unbind();

        m_triIndexCount = 0;
        m_lineBatches.clear();
    }
}
