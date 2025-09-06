#include "OxygenRender/Shader.h"
namespace OxyRender
{

    IShader::IShader(std::string name, std::string path_vertex, std::string path_fragment)
        : m_name(std::move(name)),
          m_path_vertex(std::move(path_vertex)),
          m_path_fragment(std::move(path_fragment)),
          m_use_source_code(false)
    {
    }

    IShader::IShader(std::string name, std::string vertex_source, std::string fragment_source, bool from_source)
        : m_name(std::move(name)),
          m_vertex_source(std::move(vertex_source)),
          m_fragment_source(std::move(fragment_source)),
          m_use_source_code(from_source)
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

    OpenGLShader::OpenGLShader(std::string name, std::string vertex_source, std::string fragment_source, bool from_source)
        : IShader(std::move(name), std::move(vertex_source), std::move(fragment_source), from_source)
    {
        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, m_vertex_source);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, m_fragment_source);

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

    std::string OpenGLShader::loadFile(const std::string &path)
    {
        std::ifstream file(path);
        if (!file.is_open())
            throw std::runtime_error("Failed to open shader file: " + path);

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    GLuint OpenGLShader::compileShader(GLenum type, const std::string &source)
    {
        GLuint shader = glCreateShader(type);
        const char *src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char log[512];
            glGetShaderInfoLog(shader, 512, nullptr, log);
            glDeleteShader(shader);
            throw std::runtime_error(std::string("Shader compilation failed: ") + log);
        }
        return shader;
    }
    void OpenGLShader::use()
    {
        glUseProgram(m_program_id);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_program_id);
    }
    void OpenGLShader::setMat4(const std::string &name, const glm::mat4 &mat)
    {
        use();
        GLint location = glGetUniformLocation(m_program_id, name.c_str());
        if (location != -1)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
        }
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
    std::unique_ptr<IShader> ShaderFactory::create(std::string name, std::string path_vertex, std::string path_fragment)
    {
        switch (Backends::OXYG_CurrentBackend)
        {
        case RendererBackend::OpenGL:
            return std::make_unique<OpenGLShader>(name, path_vertex, path_fragment);
        default:
            return nullptr;
        }
    }

    std::unique_ptr<IShader> ShaderFactory::createFromSource(std::string name, std::string vertex_source, std::string fragment_source)
    {
        switch (Backends::OXYG_CurrentBackend)
        {
        case RendererBackend::OpenGL:
            return std::make_unique<OpenGLShader>(name, vertex_source, fragment_source, true);
        default:
            return nullptr;
        }
    }

}
