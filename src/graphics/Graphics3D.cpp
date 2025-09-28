#include "OxygenRender/Graphics3D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using namespace MathLite;
namespace OxyRender
{

    // 硬编码的着色器源码
    const char *Graphics3D::m_vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aNormal;

out vec3 vFragPos;
out vec3 vNormal;
out vec4 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float uPointSize; // point size supplied from CPU when drawing points

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    vFragPos = worldPos.xyz;
    
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    
    vColor = aColor;
    
    gl_Position = projection * view * worldPos;
    gl_PointSize = uPointSize; // effective when rendering GL_POINTS
}
)";

    const char *Graphics3D::m_fragmentShaderSrc = R"(
#version 330 core
in vec3 vFragPos;
in vec3 vNormal;
in vec4 vColor;

out vec4 FragColor;

uniform vec3 lightPos = vec3(5.0, 5.0, 5.0);
uniform vec3 viewPos;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);

void main()
{
    // 环境光
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // 漫反射
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // 镜面反射
    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * vColor.rgb;
    FragColor = vec4(result, vColor.a);
}
)";
    Graphics3D::Graphics3D(Window &window, Renderer &renderer)
        : m_window(window),
          m_renderer(renderer),
          m_camera(glm::vec3(0.0f, 1.5f, 5.0f)),
          m_shader("default3D", m_vertexShaderSrc, m_fragmentShaderSrc),
          m_vbo(BufferType::Vertex, BufferUsage::DynamicDraw),
          m_ebo(BufferType::Index, BufferUsage::DynamicDraw)
    {
        // 顶点布局
        VertexLayout layout;
        layout.addAttribute("aPos", 0, VertexAttribType::Float3);
        layout.addAttribute("aColor", 1, VertexAttribType::Float4);
        layout.addAttribute("aNormal", 2, VertexAttribType::Float3);

        m_vao.setVertexBuffer(m_vbo, layout);
        m_vao.setIndexBuffer(m_ebo);

        m_renderer.setCapability(RenderCapability::DepthTest, true);
        m_renderer.setCapability(RenderCapability::Blend, true);
        m_renderer.setBlendFunc(RenderBlendFunc::SrcAlpha, RenderBlendFunc::OneMinusSrcAlpha);

        // 默认相机参数
        m_camera.setZoom(45.0f);
    }

    Camera &Graphics3D::getCamera()
    {
        return m_camera;
    }

    void Graphics3D::clear()
    {
        m_renderer.clear();
    }

    void Graphics3D::setClearColor(const OxyColor &color)
    {
        m_renderer.setClearColor(color);
    }

    void Graphics3D::begin()
    {
        m_triVertices.clear();
        m_triVertices.reserve(4096);
        m_triIndices.clear();
        m_triIndices.reserve(6144);
        m_triIndexCount = 0;

        m_lineBatches.clear();
        m_pointBatches.clear();
    }

    void Graphics3D::drawTriangle(const Vec3 &p1,
                                  const Vec3 &p2,
                                  const Vec3 &p3,
                                  OxyColor color)
    {
        auto n = (p2 - p1).cross(p3 - p1).normalize();
        // glm::vec3 n = glm::normalize(glm::cross(p2 - p1, p3 - p1));
        unsigned int start = (unsigned int)m_triVertices.size();
        m_triVertices.push_back({p1, color, n});
        m_triVertices.push_back({p2, color, n});
        m_triVertices.push_back({p3, color, n});

        m_triIndices.push_back(start + 0);
        m_triIndices.push_back(start + 1);
        m_triIndices.push_back(start + 2);
        m_triIndexCount += 3;
    }

    void Graphics3D::drawLine(const Vec3 &p1,
                              const Vec3 &p2,
                              OxyColor color,
                              float thickness)
    {
        LineBatch *batch = nullptr;
        for (auto &b : m_lineBatches)
        {
            if (std::fabs(b.thickness - thickness) < 0.001f)
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

        unsigned int start = (unsigned int)batch->vertices.size();

        batch->vertices.push_back({p1, color, Vec3::Zero()});
        batch->vertices.push_back({p2, color, Vec3::Zero()});
        batch->indices.push_back(start + 0);
        batch->indices.push_back(start + 1);
        batch->indexCount += 2;
    }

    void Graphics3D::drawPoints(const std::vector<Vec3> &points,
                                float size,
                                const OxyColor &color)
    {
        if (points.empty())
            return;

        PointBatch *batch = nullptr;
        for (auto &b : m_pointBatches)
        {
            if (std::fabs(b.size - size) < 0.001f &&
                b.color.r == color.r && b.color.g == color.g &&
                b.color.b == color.b && b.color.a == color.a)
            {
                batch = &b;
                break;
            }
        }
        if (!batch)
        {
            m_pointBatches.push_back(PointBatch{size, color});
            batch = &m_pointBatches.back();
        }

        batch->vertices.reserve(batch->vertices.size() + points.size());
        for (const auto &p : points)
        {
            batch->vertices.push_back({p, color, Vec3::Zero()});
        }
    }

    void Graphics3D::drawPlane(const Vec3 &center,
                               const Vec3 &inNormal,
                               const Vec2 &size,
                               const OxyColor &color)
    {
        Vec3 N = inNormal;
        float nlen = N.length();
        if (nlen < 1e-6f)
        {
            N = Vec3::Up();
        }
        else
        {
            N /= nlen;
        }

        // glm::vec3 ref = (std::fabs(N.y) < 0.9f) ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
        Vec3 ref = (std::fabs(N.y) < 0.9f) ? Vec3::Up() : Vec3::Right();
        // glm::vec3 T = glm::normalize(glm::cross(ref, N));
        Vec3 T = (ref.cross(N)).normalize();

        // if (glm::dot(T, T) < 1e-12f)
        // {
        //     ref = glm::vec3(0, 0, 1);
        //     T = glm::normalize(glm::cross(ref, N));
        // }
        if (T.dot(T) < 1e-12f)
        {
            ref = Vec3::Forward();
            T = (ref.cross(N)).normalize();
        }
        Vec3 B = (N.cross(T)).normalize();
        // glm::vec3 B = glm::normalize(glm::cross(N, T));

        float hx = size.x * 0.5f;
        float hy = size.y * 0.5f;

        Vec3 p0 = center - T * hx - B * hy; // 左下
        Vec3 p1 = center + T * hx - B * hy; // 右下
        Vec3 p2 = center + T * hx + B * hy; // 右上
        Vec3 p3 = center - T * hx + B * hy; // 左上

        unsigned base = (unsigned)m_triVertices.size();
        m_triVertices.push_back({p0, color, N});
        m_triVertices.push_back({p1, color, N});
        m_triVertices.push_back({p2, color, N});
        m_triVertices.push_back({p3, color, N});

        // 两个三角形：p0->p1->p2, p0->p2->p3（CCW）
        m_triIndices.push_back(base + 0);
        m_triIndices.push_back(base + 1);
        m_triIndices.push_back(base + 2);

        m_triIndices.push_back(base + 0);
        m_triIndices.push_back(base + 2);
        m_triIndices.push_back(base + 3);

        m_triIndexCount += 6;
    }

    void Graphics3D::drawBox(const Vec3 &center, const Vec3 &size, const OxyColor &color)
    {
        Vec3 half = size * 0.5f;

        Vec3 p[8] = {
            center + Vec3(-half.x, -half.y, -half.z),
            center + Vec3(half.x, -half.y, -half.z),
            center + Vec3(half.x, half.y, -half.z),
            center + Vec3(-half.x, half.y, -half.z),
            center + Vec3(-half.x, -half.y, half.z),
            center + Vec3(half.x, -half.y, half.z),
            center + Vec3(half.x, half.y, half.z),
            center + Vec3(-half.x, half.y, half.z)};

        // 每个面 push 顶点并按当前顶点基址写索引
        auto pushFace = [&](int i0, int i1, int i2, int i3, const Vec3 &normal)
        {
            unsigned int base = (unsigned int)m_triVertices.size();
            m_triVertices.push_back({p[i0], color, normal});
            m_triVertices.push_back({p[i1], color, normal});
            m_triVertices.push_back({p[i2], color, normal});
            m_triVertices.push_back({p[i2], color, normal});
            m_triVertices.push_back({p[i3], color, normal});
            m_triVertices.push_back({p[i0], color, normal});

            // 对应索引
            m_triIndices.push_back(base + 0);
            m_triIndices.push_back(base + 1);
            m_triIndices.push_back(base + 2);
            m_triIndices.push_back(base + 3);
            m_triIndices.push_back(base + 4);
            m_triIndices.push_back(base + 5);
            m_triIndexCount += 6;
        };

        // 六个面（法线朝外）
        pushFace(0, 1, 2, 3, {0, 0, -1}); //  -Z
        pushFace(4, 5, 6, 7, {0, 0, 1});  //  +Z
        pushFace(0, 4, 7, 3, {-1, 0, 0}); //  -X
        pushFace(1, 5, 6, 2, {1, 0, 0});  //  +X
        pushFace(3, 2, 6, 7, {0, 1, 0});  //  +Y
        pushFace(0, 1, 5, 4, {0, -1, 0}); //  -Y
    }

    void Graphics3D::drawSphere(const Vec3 &center, float radius,
                                int stacks, int slices, const OxyColor &color)
    {
        if (stacks < 2)
            stacks = 2;
        if (slices < 3)
            slices = 3;

        for (int i = 0; i < stacks; ++i)
        {
            float phi1 = glm::pi<float>() * float(i) / float(stacks);
            float phi2 = glm::pi<float>() * float(i + 1) / float(stacks);

            for (int j = 0; j < slices; ++j)
            {
                float theta1 = glm::two_pi<float>() * float(j) / float(slices);
                float theta2 = glm::two_pi<float>() * float(j + 1) / float(slices);

                Vec3 p1 = center + radius * Vec3(
                                                std::sin(phi1) * std::cos(theta1),
                                                std::cos(phi1),
                                                std::sin(phi1) * std::sin(theta1));
                Vec3 p2 = center + radius * Vec3(
                                                std::sin(phi2) * std::cos(theta1),
                                                std::cos(phi2),
                                                std::sin(phi2) * std::sin(theta1));
                Vec3 p3 = center + radius * Vec3(
                                                std::sin(phi2) * std::cos(theta2),
                                                std::cos(phi2),
                                                std::sin(phi2) * std::sin(theta2));
                Vec3 p4 = center + radius * Vec3(
                                                std::sin(phi1) * std::cos(theta2),
                                                std::cos(phi1),
                                                std::sin(phi1) * std::sin(theta2));

                Vec3 n1 = (p1 - center).normalize();
                Vec3 n2 = (p2 - center).normalize();
                Vec3 n3 = (p3 - center).normalize();
                Vec3 n4 = (p4 - center).normalize();

                unsigned int base = (unsigned int)m_triVertices.size();
                // 第一个三角形
                m_triVertices.push_back({p1, color, n1});
                m_triVertices.push_back({p2, color, n2});
                m_triVertices.push_back({p3, color, n3});
                m_triIndices.push_back(base + 0);
                m_triIndices.push_back(base + 1);
                m_triIndices.push_back(base + 2);
                m_triIndexCount += 3;

                // 第二个三角形
                m_triVertices.push_back({p3, color, n3});
                m_triVertices.push_back({p4, color, n4});
                m_triVertices.push_back({p1, color, n1});
                m_triIndices.push_back(base + 3);
                m_triIndices.push_back(base + 4);
                m_triIndices.push_back(base + 5);
                m_triIndexCount += 3;
            }
        }
    }

    void Graphics3D::drawCylinder(const Vec3 &center,
                                  float radius,
                                  float height,
                                  int slices,
                                  const OxyColor &color,
                                  bool capped)
    {
        if (radius <= 0.0f || height <= 0.0f)
            return;
        if (slices < 3)
            slices = 3;

        float halfH = 0.5f * height;
        float yTop = center.y + halfH;
        float yBottom = center.y - halfH;

        for (int j = 0; j < slices; ++j)
        {
            float theta1 = glm::two_pi<float>() * float(j) / float(slices);
            float theta2 = glm::two_pi<float>() * float(j + 1) / float(slices);

            Vec3 p1(center.x + radius * std::cos(theta1), yBottom, center.z + radius * std::sin(theta1));
            Vec3 p2(center.x + radius * std::cos(theta2), yBottom, center.z + radius * std::sin(theta2));
            Vec3 p3(center.x + radius * std::cos(theta2), yTop, center.z + radius * std::sin(theta2));
            Vec3 p4(center.x + radius * std::cos(theta1), yTop, center.z + radius * std::sin(theta1));

            // Vec3 n1 = glm::normalize(glm::vec3(p1.x - center.x, 0.0f, p1.z - center.z));
            // Vec3 n2 = glm::normalize(glm::vec3(p2.x - center.x, 0.0f, p2.z - center.z));
            // Vec3 n3 = glm::normalize(glm::vec3(p3.x - center.x, 0.0f, p3.z - center.z));
            // Vec3 n4 = glm::normalize(glm::vec3(p4.x - center.x, 0.0f, p4.z - center.z));

            Vec3 n1 = Vec3(p1.x - center.x, 0.0f, p1.z - center.z).normalize();
            Vec3 n2 = Vec3(p2.x - center.x, 0.0f, p2.z - center.z).normalize();
            Vec3 n3 = Vec3(p3.x - center.x, 0.0f, p3.z - center.z).normalize();
            Vec3 n4 = Vec3(p4.x - center.x, 0.0f, p4.z - center.z).normalize();

            unsigned int base = (unsigned int)m_triVertices.size();

            m_triVertices.push_back({p1, color, n1});
            m_triVertices.push_back({p2, color, n2});
            m_triVertices.push_back({p3, color, n3});
            m_triIndices.push_back(base + 0);
            m_triIndices.push_back(base + 1);
            m_triIndices.push_back(base + 2);
            m_triIndexCount += 3;

            m_triVertices.push_back({p3, color, n3});
            m_triVertices.push_back({p4, color, n4});
            m_triVertices.push_back({p1, color, n1});
            m_triIndices.push_back(base + 3);
            m_triIndices.push_back(base + 4);
            m_triIndices.push_back(base + 5);
            m_triIndexCount += 3;
        }

        if (capped)
        {

            {
                Vec3 nTop(0.0f, 1.0f, 0.0f);
                unsigned int centerBase = (unsigned int)m_triVertices.size();

                m_triVertices.push_back({Vec3(center.x, yTop, center.z), color, nTop});

                for (int j = 0; j < slices; ++j)
                {
                    float theta = glm::two_pi<float>() * float(j) / float(slices);
                    Vec3 p(center.x + radius * std::cos(theta), yTop, center.z + radius * std::sin(theta));
                    m_triVertices.push_back({p, color, nTop});
                }

                for (int j = 0; j < slices; ++j)
                {
                    unsigned int i0 = centerBase + 0;
                    unsigned int i1 = centerBase + 1 + j;
                    unsigned int i2 = centerBase + 1 + ((j + 1) % slices);
                    m_triIndices.push_back(i0);
                    m_triIndices.push_back(i1);
                    m_triIndices.push_back(i2);
                    m_triIndexCount += 3;
                }
            }

            {
                Vec3 nBot(0.0f, -1.0f, 0.0f);
                unsigned int centerBase = (unsigned int)m_triVertices.size();

                m_triVertices.push_back({Vec3(center.x, yBottom, center.z), color, nBot});

                for (int j = 0; j < slices; ++j)
                {
                    float theta = glm::two_pi<float>() * float(j) / float(slices);
                    Vec3 p(center.x + radius * std::cos(theta), yBottom, center.z + radius * std::sin(theta));
                    m_triVertices.push_back({p, color, nBot});
                }

                for (int j = 0; j < slices; ++j)
                {
                    unsigned int i0 = centerBase + 0;
                    unsigned int i1 = centerBase + 1 + ((j + 1) % slices);
                    unsigned int i2 = centerBase + 1 + j;
                    m_triIndices.push_back(i0);
                    m_triIndices.push_back(i1);
                    m_triIndices.push_back(i2);
                    m_triIndexCount += 3;
                }
            }
        }
    }

    void Graphics3D::drawFunction(
        const Vec2 &xDomain,
        const Vec2 &zDomain,
        const std::function<float(float, float)> &func,
        const OxyColor &color,
        const float &dx,
        const float &dz)
    {
        Vec2 xRange = xDomain;
        Vec2 zRange = zDomain;
        int nx = static_cast<int>((xRange.y - xRange.x) / dx);
        int nz = static_cast<int>((zRange.y - zRange.x) / dz);

        // 调整 xMax，yMax 以确保整除避免边缘不完整
        xRange.y = xRange.x + nx * dx;
        zRange.y = zRange.x + nz * dz;

        for (int i = 0; i < nx; ++i)
        {
            for (int j = 0; j < nz; ++j)
            {
                float x1 = xRange.x + i * dx;
                float x2 = xRange.x + (i + 1) * dx;
                float z1 = zRange.x + j * dz;
                float z2 = zRange.x+ (j + 1) * dz;

                Vec3 p1(x1, func(x1, z1), z1);
                Vec3 p2(x2, func(x2, z1), z1);
                Vec3 p3(x2, func(x2, z2), z2);
                Vec3 p4(x1, func(x1, z2), z2);

                drawTriangle(p1, p2, p4, color);
                drawTriangle(p2, p3, p4, color);
            }
        }
    }

    void Graphics3D::flush()
    {
        if (m_triIndexCount == 0 && m_lineBatches.empty() && m_pointBatches.empty())
            return;

        m_renderer.setCapability(RenderCapability::Multisample, true);
        m_renderer.setCapability(RenderCapability::Blend, true);
        m_renderer.setCapability(RenderCapability::DepthTest, true);
        m_renderer.setCapability(RenderCapability::StencilTest, false);

        Shader *shaderToUse = m_customShader ? m_customShader : &m_shader;
        shaderToUse->use();

        // 3D MVP
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = m_camera.getViewMatrix();
        glm::mat4 projection = m_camera.getPerspectiveProjectionMatrix(m_window.getWidth(), m_window.getHeight());

        shaderToUse->setUniformData("model", &model, sizeof(glm::mat4));
        shaderToUse->setUniformData("view", &view, sizeof(glm::mat4));
        shaderToUse->setUniformData("projection", &projection, sizeof(glm::mat4));

        // 设置光照和相机位置
        glm::vec3 lightPos(5.0f, 5.0f, 5.0f);
        glm::vec3 camPos = m_camera.getPosition();
        shaderToUse->setUniformData("lightPos", &lightPos, sizeof(glm::vec3));
        shaderToUse->setUniformData("viewPos", &camPos, sizeof(glm::vec3));

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

        // 面绘制
        if (m_triIndexCount > 0)
        {
            m_vbo.setData(m_triVertices.data(), m_triVertices.size() * sizeof(Vertex));
            m_ebo.setData(m_triIndices.data(), m_triIndices.size() * sizeof(unsigned int));
            m_renderer.drawTriangles(m_vao, m_triIndexCount);
        }

        // 点绘制
        if (!m_pointBatches.empty())
        {
            m_renderer.setCapability(RenderCapability::ProgramPointSize, true);
            for (auto &pb : m_pointBatches)
            {
                if (pb.vertices.empty())
                    continue;
                m_vbo.setData(pb.vertices.data(), pb.vertices.size() * sizeof(Vertex));
                float size = pb.size;
                shaderToUse->setUniformData("uPointSize", &size, sizeof(float));
                m_renderer.drawPoints(m_vao, static_cast<uint32_t>(pb.vertices.size()));
            }
            m_renderer.setCapability(RenderCapability::ProgramPointSize, false);
        }

        m_vao.unbind();

        // 清空批次
        m_triIndexCount = 0;
        m_triVertices.clear();
        m_triIndices.clear();
        m_lineBatches.clear();
        m_pointBatches.clear();
    }
}
