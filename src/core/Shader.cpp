#include "OxygenRender/Shader.h"
namespace OxyRender
{

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

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void OpenGLShader::use()
    {
        glUseProgram(m_program_id);
    }

    GLuint OpenGLShader::getID()
    {
        return m_program_id;
    }
    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_program_id);
    }
    void OpenGLShader::setUniformData(const std::string &name, const void *data, size_t size)
    {
        GLint location = glGetUniformLocation(m_program_id, name.c_str());
        if (location == -1)
            throw std::runtime_error("Uniform not found: " + name);

        if (size == sizeof(float) * 1)
            glUniform1fv(location, 1, (const GLfloat *)data);
        else if (size == sizeof(float) * 2)
            glUniform2fv(location, 1, (const GLfloat *)data);
        else if (size == sizeof(float) * 3)
            glUniform3fv(location, 1, (const GLfloat *)data);
        else if (size == sizeof(float) * 4)
            glUniform4fv(location, 1, (const GLfloat *)data);
        else if (size == sizeof(float) * 9)
            glUniformMatrix3fv(location, 1, GL_FALSE, (const GLfloat *)data);
        else if (size == sizeof(float) * 16)
            glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)data);
        else if (size == sizeof(int))
            glUniform1i(location, *(const GLint *)data);
        else
            throw std::runtime_error("Unsupported uniform size: " + std::to_string(size));
    }

}
