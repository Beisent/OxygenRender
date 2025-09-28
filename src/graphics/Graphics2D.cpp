#include "OxygenRender/Graphics2D.h"
#include <glm/glm.hpp>
namespace OxyRender
{
    glm::vec2 sVec2ToGlm(const MathLite::Vec2 &vec)
    {
        return glm::vec2(vec.x, vec.y);
    }
    // 硬编码的着色器源码
    const char *Graphics2D::m_vertexShaderSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec4 aColor;

    out vec4 vColor;
    out vec2 vLocalPos;   

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0);

        vColor = aColor;
        vLocalPos = aPos.xy;   
    }
    )";

    const char *Graphics2D::m_fragmentShaderSrc = R"(
    #version 330 core
    in vec4 vColor;
    out vec4 FragColor;

    void main()
    {
        FragColor = vColor;
    }
    )";

    const char *Graphics2D::m_textureVertexShaderSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec4 aColor;
    layout(location = 2) in vec2 aTexCoord;

    out vec4 vColor;
    out vec2 vTexCoord;
    out vec2 vLocalPos;   

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0);

        vColor = aColor;
        vTexCoord = aTexCoord;
        vLocalPos = aPos.xy;   
    }
    )";

    const char *Graphics2D::m_textureFragmentShaderSrc = R"(
    #version 330 core
    in vec4 vColor;
    in vec2 vTexCoord;
    out vec4 FragColor;

    uniform sampler2D uTexture;
    uniform bool uUseTexture;

    void main()
    {
        if (uUseTexture)
        {
            FragColor = texture(uTexture, vTexCoord) * vColor;
        }
        else
        {
            FragColor = vColor;
        }
    }
    )";
    Graphics2D::Graphics2D(Window &window, Renderer &renderer)
        : m_window(window),
          m_renderer(renderer),
          m_camera(glm::vec3(0, 0, 10.0f)),
          m_shader("default", m_vertexShaderSrc, m_fragmentShaderSrc),
          m_textureShader("texture", m_textureVertexShaderSrc, m_textureFragmentShaderSrc),
          m_vbo(BufferType::Vertex, BufferUsage::DynamicDraw),
          m_ebo(BufferType::Index, BufferUsage::DynamicDraw)
    {
        // 创建顶点布局（支持纹理坐标）
        VertexLayout layout;
        layout.addAttribute("aPos", 0, VertexAttribType::Float3);
        layout.addAttribute("aColor", 1, VertexAttribType::Float4);
        layout.addAttribute("aTexCoord", 2, VertexAttribType::Float2);

        // 设置数据
        m_vao.setVertexBuffer(m_vbo, layout);
        m_vao.setIndexBuffer(m_ebo);

        // 初始化渲染
        m_renderer.setCapability(RenderCapability::DepthTest, true);
        m_renderer.setCapability(RenderCapability::Blend, true);
        m_renderer.setBlendFunc(RenderBlendFunc::SrcAlpha, RenderBlendFunc::OneMinusSrcAlpha);

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
        m_textureBatches.clear();
    }

    // 纹理相关方法实现
    void Graphics2D::setTexture(const Texture2D *texture)
    {
        m_currentTexture = texture;
    }

    void Graphics2D::clearTexture()
    {
        m_currentTexture = nullptr;
    }

    void Graphics2D::drawRect(float x, float y, float width, float height, OxyColor color)
    {
        Vertex v0 = {{x, y, 0.0f}, color, {0.0f, 0.0f}};
        Vertex v1 = {{x + width, y, 0.0f}, color, {1.0f, 0.0f}};
        Vertex v2 = {{x + width, y + height, 0.0f}, color, {1.0f, 1.0f}};
        Vertex v3 = {{x, y + height, 0.0f}, color, {0.0f, 1.0f}};

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
        Vertex v1 = {{x1, y1, 0.0f}, color, {0.0f, 0.0f}};
        Vertex v2 = {{x2, y2, 0.0f}, color, {0.5f, 1.0f}};
        Vertex v3 = {{x3, y3, 0.0f}, color, {1.0f, 0.0f}};

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

        Vertex v1 = {{x1, y1, 0.0f}, color, {0.0f, 0.0f}};
        Vertex v2 = {{x2, y2, 0.0f}, color, {1.0f, 1.0f}};

        unsigned int startIndex = (unsigned int)batch->vertices.size();
        batch->vertices.push_back(v1);
        batch->vertices.push_back(v2);

        batch->indices.push_back(startIndex + 0);
        batch->indices.push_back(startIndex + 1);

        batch->indexCount += 2;
    }
    void Graphics2D::drawLines(const std::vector<MathLite::Vec2> &points, OxyColor color, float thickness)
    {
        for (int i = 0; i < points.size() - 1; i++)
        {
            drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, color, thickness);
        }
    }

    void Graphics2D::drawCircle(float cx, float cy, float radius, OxyColor color, int segments)
    {
        drawEllipse(cx, cy, radius, radius, color, segments);
    }

    void Graphics2D::drawCircleOutline(float cx, float cy, float radius, OxyColor color, int segments, float thickness)
    {
        drawEllipseOutline(cx, cy, radius, radius, color, segments, thickness);
    }

    void Graphics2D::drawEllipse(float cx, float cy, float radiusX, float radiusY,
                                 OxyColor color, int segments)
    {
        if (segments < 3)
            segments = 3;

        unsigned int startIndex = (unsigned int)m_triVertices.size();
        m_triVertices.push_back({{cx, cy, 0.0f}, color, {0.5f, 0.5f}});

        for (int i = 0; i <= segments; i++)
        {
            float angle = (float)i / segments * 2.0f * 3.14159265358979323846f;
            float x = cx + cos(angle) * radiusX;
            float y = cy + sin(angle) * radiusY;
            float u = 0.5f + 0.5f * cos(angle);
            float v = 0.5f + 0.5f * sin(angle);
            m_triVertices.push_back({{x, y, 0.0f}, color, {u, v}});
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

    void Graphics2D::drawPolygon(const std::vector<MathLite::Vec2> &points, OxyColor color)
    {
        size_t n = points.size();
        if (n < 3)
            return;

        unsigned int startIndex = static_cast<unsigned int>(m_triVertices.size());

        for (const auto &p : points)
        {
            m_triVertices.push_back({{p.x, p.y, 0.0f}, color, {0.5f, 0.5f}});
        }

        for (size_t i = 1; i < n - 1; i++)
        {
            m_triIndices.push_back(startIndex);
            m_triIndices.push_back(startIndex + i);
            m_triIndices.push_back(startIndex + i + 1);
            m_triIndexCount += 3;
        }
    }

    void Graphics2D::drawPolygonOutline(const std::vector<MathLite::Vec2> &points, OxyColor color, float thickness)
    {
        if (points.size() < 2)
            return;

        for (size_t i = 0; i < points.size(); i++)
        {
            auto p1_ = points[i];
            glm::vec2 p1 = {p1_.x, p1_.y};
            auto p2_ = points[(i + 1) % points.size()];
            glm::vec2 p2 = {p2_.x, p2_.y};
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
    void Graphics2D::drawRect(float x, float y, float width, float height, const Texture2D &texture, OxyColor tintColor)
    {
        // 查找或创建对应的纹理批次
        TextureBatch *batch = nullptr;
        for (auto &b : m_textureBatches)
        {
            if (b.texture == &texture)
            {
                batch = &b;
                break;
            }
        }

        if (!batch)
        {
            m_textureBatches.push_back(TextureBatch{&texture});
            batch = &m_textureBatches.back();
        }

        unsigned int startIndex = static_cast<unsigned int>(batch->vertices.size());
        batch->vertices.push_back({{x, y, 0.0f}, tintColor, {0.0f, 0.0f}});                  // 左下
        batch->vertices.push_back({{x + width, y, 0.0f}, tintColor, {1.0f, 0.0f}});          // 右下
        batch->vertices.push_back({{x + width, y + height, 0.0f}, tintColor, {1.0f, 1.0f}}); // 右上
        batch->vertices.push_back({{x, y + height, 0.0f}, tintColor, {0.0f, 1.0f}});         // 左上

        // 添加索引（两个三角形）
        batch->indices.push_back(startIndex + 0);
        batch->indices.push_back(startIndex + 1);
        batch->indices.push_back(startIndex + 2);
        batch->indices.push_back(startIndex + 2);
        batch->indices.push_back(startIndex + 3);
        batch->indices.push_back(startIndex + 0);
        batch->indexCount += 6;
    }

    void Graphics2D::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3,
                                  const Texture2D &texture, OxyColor tintColor)
    {
        // 查找或创建对应的纹理批次
        TextureBatch *batch = nullptr;
        for (auto &b : m_textureBatches)
        {
            if (b.texture == &texture)
            {
                batch = &b;
                break;
            }
        }

        if (!batch)
        {
            m_textureBatches.push_back(TextureBatch{&texture});
            batch = &m_textureBatches.back();
        }

        // 为三角形添加三个顶点
        unsigned int startIndex = static_cast<unsigned int>(batch->vertices.size());
        batch->vertices.push_back({{x1, y1, 0.0f}, tintColor, {0.0f, 0.0f}});
        batch->vertices.push_back({{x2, y2, 0.0f}, tintColor, {0.5f, 1.0f}});
        batch->vertices.push_back({{x3, y3, 0.0f}, tintColor, {1.0f, 0.0f}});

        // 添加索引
        batch->indices.push_back(startIndex + 0);
        batch->indices.push_back(startIndex + 1);
        batch->indices.push_back(startIndex + 2);
        batch->indexCount += 3;
    }

    void Graphics2D::drawPolygon(const std::vector<MathLite::Vec2> &points, const Texture2D &texture, OxyColor tintColor)
    {
        if (points.size() < 3)
            return;

        // 查找或创建对应的纹理批次
        TextureBatch *batch = nullptr;
        for (auto &b : m_textureBatches)
        {
            if (b.texture == &texture)
            {
                batch = &b;
                break;
            }
        }

        if (!batch)
        {
            m_textureBatches.push_back(TextureBatch{&texture});
            batch = &m_textureBatches.back();
        }

        // 计算多边形的中心点
        glm::vec2 center(0.0f);
        for (const auto &point : points)
        {
            center += sVec2ToGlm(point);
        }
        center /= static_cast<float>(points.size());

        // 添加中心顶点
        unsigned int startIndex = static_cast<unsigned int>(batch->vertices.size());
        batch->vertices.push_back({{center.x, center.y, 0.0f}, tintColor, {0.5f, 0.5f}});

        // 添加边界顶点
        for (size_t i = 0; i < points.size(); ++i)
        {
            float u = static_cast<float>(i) / static_cast<float>(points.size());
            batch->vertices.push_back({{points[i].x, points[i].y, 0.0f}, tintColor, {u, 0.0f}});
        }

        // 添加第一个点作为闭合
        batch->vertices.push_back({{points[0].x, points[0].y, 0.0f}, tintColor, {1.0f, 0.0f}});

        // 添加索引
        for (size_t i = 0; i < points.size(); ++i)
        {
            batch->indices.push_back(startIndex); // 中心点
            batch->indices.push_back(startIndex + 1 + i);
            batch->indices.push_back(startIndex + 1 + ((i + 1) % points.size()));
            batch->indexCount += 3;
        }
    }

    void Graphics2D::drawEllipse(float cx, float cy, float radiusX, float radiusY,
                                 const Texture2D &texture, OxyColor tintColor, int segments)
    {
        if (segments < 3)
            segments = 3;

        // 查找或创建对应的纹理批次
        TextureBatch *batch = nullptr;
        for (auto &b : m_textureBatches)
        {
            if (b.texture == &texture)
            {
                batch = &b;
                break;
            }
        }

        if (!batch)
        {
            m_textureBatches.push_back(TextureBatch{&texture});
            batch = &m_textureBatches.back();
        }

        // 添加中心顶点
        unsigned int startIndex = static_cast<unsigned int>(batch->vertices.size());
        batch->vertices.push_back({{cx, cy, 0.0f}, tintColor, {0.5f, 0.5f}});

        // 添加边界顶点
        for (int i = 0; i <= segments; i++)
        {
            float angle = (float)i / segments * 2.0f * 3.14159265358979323846f;
            float x = cx + cos(angle) * radiusX;
            float y = cy + sin(angle) * radiusY;
            // 将角度映射到纹理坐标
            float u = 0.5f + 0.5f * cos(angle);
            float v = 0.5f + 0.5f * sin(angle);
            batch->vertices.push_back({{x, y, 0.0f}, tintColor, {u, v}});
        }

        // 添加索引
        for (int i = 1; i <= segments; i++)
        {
            batch->indices.push_back(startIndex); // 中心点
            batch->indices.push_back(startIndex + i);
            batch->indices.push_back(startIndex + i + 1);
            batch->indexCount += 3;
        }
    }

    void Graphics2D::drawCircle(float cx, float cy, float radius, const Texture2D &texture,
                                OxyColor tintColor, int segments)
    {
        drawEllipse(cx, cy, radius, radius, texture, tintColor, segments);
    }
    void Graphics2D::drawAxis(OxyColor axisColor,
                              OxyColor gridColor,
                              float thickness,
                              float gridSpacing,
                              bool drawGrid)
    {
        // 投影和视图矩阵
        glm::mat4 proj = m_camera.getOrthoProjectionMatrix2D(m_window.getWidth(), m_window.getHeight());
        glm::mat4 view = m_camera.getOrthoViewMatrix2D();
        glm::mat4 vp = proj * view;
        glm::mat4 invVP = glm::inverse(vp);

        // 将 NDC 转换为屏幕坐标
        auto unproject = [&](float ndcX, float ndcY)
        {
            glm::vec4 p(ndcX, ndcY, 0.0f, 1.0f);
            p = invVP * p;
            return glm::vec2(p.x / p.w, p.y / p.w);
        };

        glm::vec2 bottomLeft = unproject(-1, -1);
        glm::vec2 topRight = unproject(1, 1);

        // 渲染网格
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
        // 渲染坐标轴
        drawArrow(bottomLeft.x, 0, topRight.x, 0, axisColor, thickness);
        drawArrow(0, bottomLeft.y, 0, topRight.y, axisColor, thickness);
    }

    // 二次贝塞尔曲线
    void Graphics2D::drawBezier(float x0, float y0, float cx, float cy, float x1,
                                float y1, OxyColor color, float thickness, int segments)
    {
        if (segments < 1)
            segments = 1;
        glm::vec2 prev(x0, y0);

        for (int i = 1; i <= segments; ++i)
        {
            float t = static_cast<float>(i) / static_cast<float>(segments);
            float u = 1.0f - t;
            glm::vec2 pt = u * u * glm::vec2(x0, y0) + 2.0f * u * t * glm::vec2(cx, cy) + t * t * glm::vec2(x1, y1);

            drawLine(prev.x, prev.y, pt.x, pt.y, color, thickness);
            prev = pt;
        }
    }

    // 三次贝塞尔曲线
    void Graphics2D::drawBezier(float x0, float y0, float c1x, float c1y, float c2x, float c2y,
                                float x1, float y1, OxyColor color, float thickness, int segments)
    {
        if (segments < 1)
            segments = 1;
        glm::vec2 prev(x0, y0);

        for (int i = 1; i <= segments; ++i)
        {
            float t = static_cast<float>(i) / static_cast<float>(segments);
            float u = 1.0f - t;

            // B(t) = u^3 P0 + 3 u^2 t P1 + 3 u t^2 P2 + t^3 P3
            glm::vec2 pt = (u * u * u) * glm::vec2(x0, y0) + 3.0f * (u * u) * t * glm::vec2(c1x, c1y) + 3.0f * u * (t * t) * glm::vec2(c2x, c2y) + (t * t * t) * glm::vec2(x1, y1);

            drawLine(prev.x, prev.y, pt.x, pt.y, color, thickness);
            prev = pt;
        }
    }
    void Graphics2D::drawFunction(const float &xStart, const float &xEnd,
                                  const std::function<float(float)> &func,
                                  const OxyColor &color,
                                  const float &dx,
                                  const float &thickness)
    {
        for (float x = xStart; x < xEnd; x += dx)
        {
            drawLine(x, func(x), x + dx, func(x + dx), color, thickness);
        }
    }

    void Graphics2D::flush()
    {
        if (m_triIndexCount == 0 && m_lineBatches.empty())
            return;

        m_renderer.setCapability(RenderCapability::Multisample, true);
        m_renderer.setCapability(RenderCapability::Blend, true);
        m_renderer.setCapability(RenderCapability::DepthTest, false);
        m_renderer.setCapability(RenderCapability::StencilTest, false);

        Shader *shaderToUse = m_customShader ? m_customShader : &m_shader;
        shaderToUse->use();

        // MVP变换
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = m_camera.getOrthoViewMatrix2D();
        glm::mat4 projection = m_camera.getOrthoProjectionMatrix2D(m_window.getWidth(), m_window.getHeight());

        shaderToUse->setUniformData("model", &model, sizeof(glm::mat4));
        shaderToUse->setUniformData("view", &view, sizeof(glm::mat4));
        shaderToUse->setUniformData("projection", &projection, sizeof(glm::mat4));

        m_vao.bind();

        // 线绘制
        for (auto &batch : m_lineBatches)
        {
            if (batch.indexCount == 0)
                continue;

            m_vbo.setData(batch.vertices.data(), batch.vertices.size() * sizeof(Vertex));
            m_ebo.setData(batch.indices.data(), batch.indices.size() * sizeof(unsigned int));
            m_renderer.drawLines(m_vao, batch.indexCount, batch.thickness);
        }

        // 三角形绘制
        if (m_triIndexCount > 0)
        {
            m_vbo.setData(m_triVertices.data(), m_triVertices.size() * sizeof(Vertex));
            m_ebo.setData(m_triIndices.data(), m_triIndices.size() * sizeof(unsigned int));
            m_renderer.drawTriangles(m_vao, m_triIndexCount);
        }

        m_vao.unbind();

        m_triIndexCount = 0;
        m_lineBatches.clear();

        // 刷新纹理批次
        flushTextureBatches();
    }

    void Graphics2D::flushTextureBatches()
    {
        if (m_textureBatches.empty())
            return;
        Shader *shaderToUse = m_customTextureShader ? m_customTextureShader : &m_textureShader;
        shaderToUse->use();

        // MVP变换
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = m_camera.getOrthoViewMatrix2D();
        glm::mat4 projection = m_camera.getOrthoProjectionMatrix2D(m_window.getWidth(), m_window.getHeight());

        shaderToUse->setUniformData("model", &model, sizeof(glm::mat4));
        shaderToUse->setUniformData("view", &view, sizeof(glm::mat4));
        shaderToUse->setUniformData("projection", &projection, sizeof(glm::mat4));

        m_vao.bind();

        for (auto &batch : m_textureBatches)
        {
            if (batch.indexCount == 0 || !batch.texture)
                continue;

            // 绑定纹理
            batch.texture->bind(0);
            int useTexture = 1;
            shaderToUse->setUniformData("uUseTexture", &useTexture, sizeof(int));

            // 设置顶点数据
            m_vbo.setData(batch.vertices.data(), batch.vertices.size() * sizeof(Vertex));
            m_ebo.setData(batch.indices.data(), batch.indices.size() * sizeof(unsigned int));

            // 绘制
            m_renderer.drawTriangles(m_vao, batch.indexCount);
        }

        m_vao.unbind();
    }

}
