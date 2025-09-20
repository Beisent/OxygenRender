#include "OxygenRender/Skybox.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

namespace OxyRender
{

    const char *Skybox::s_vertexShaderSrc = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    out vec3 TexCoords;

    uniform mat4 projection;
    uniform mat4 view;

    void main()
    {
        TexCoords = aPos;
        vec4 pos = projection * view * vec4(aPos, 1.0);
        gl_Position = pos.xyww;
    }
    )";

    const char *Skybox::s_fragmentShaderSrc = R"(
    #version 330 core
    out vec4 FragColor;

    in vec3 TexCoords;

    uniform samplerCube skybox;

    void main()
    {
        FragColor = texture(skybox, TexCoords);
    }
    )";

    Skybox::Skybox(Renderer &renderer, const std::vector<std::string> &faces)
        : m_renderer(renderer), m_cubemap(faces),
          m_vbo(BufferType::Vertex, BufferUsage::StaticDraw) 
    {
        setupMesh();

    }

    Skybox::~Skybox()
    {
    }

    void Skybox::setupMesh()
    {
        // 天空盒顶点数据
        float skyboxVertices[] = {
            // 后面
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            // 前面
            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            // 左面
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,

            // 右面
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,

            // 底面
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,

            // 顶面
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f};

  
        m_vao.bind();

        m_vbo.setData(skyboxVertices, sizeof(skyboxVertices));


        VertexLayout layout;
        layout.addAttribute("aPos", 0, VertexAttribType::Float3);
        m_vao.setVertexBuffer(m_vbo, layout);

        m_vao.unbind();
    }

    void Skybox::draw(Shader &shader, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
    {
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        shader.use();

        glm::mat4 view = glm::mat4(glm::mat3(viewMatrix));
        GLint viewLoc = glGetUniformLocation(shader.getID(), "view");
        GLint projLoc = glGetUniformLocation(shader.getID(), "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        m_cubemap.bind(0);

        m_vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36); 
        m_vao.unbind();

        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
    }

    Shader Skybox::createDefaultShader()
    {
        return Shader("SkyboxShader", s_vertexShaderSrc, s_fragmentShaderSrc);
    }

} // namespace OxyRender