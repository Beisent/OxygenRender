#include "OxygenRender/Shader.h"
namespace OxyRender
{
    std::string loadFile(const std::string &filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open shader file: " + filepath);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    IShader::IShader(std::string name, std::string path_vertex, std::string path_fragment)
        : m_name(std::move(name)),
          m_path_vertex(std::move(path_vertex)),
          m_path_fragment(std::move(path_fragment))
    {
    }

    OpenGLShader::OpenGLShader(std::string name, std::string path_vertex, std::string path_fragment)
        : IShader(std::move(name), std::move(path_vertex), std::move(path_fragment))
    {
        std::string vertexCode = loadFile(m_path_vertex);
        std::string fragmentCode = loadFile(m_path_fragment);

        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

        m_program_id = glCreateProgram();
        glAttachShader(m_program_id, vertexShader);
        glAttachShader(m_program_id, fragmentShader);
        glLinkProgram(m_program_id);

        GLint success;
        glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            char log[512];
            glGetProgramInfoLog(m_program_id, 512, nullptr, log);
            glDeleteProgram(m_program_id);
            throw std::runtime_error(std::string("Program linking failed: ") + log);
        }

        // 删除临时 shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void OpenGLShader::use()
    {
        glUseProgram(m_program_id);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_program_id);
    }
}
