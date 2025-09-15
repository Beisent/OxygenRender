#include "OxygenRender/Shader.h"
#include <glad/glad.h>
namespace OxyRender
{

    IShader::IShader(std::string name, std::string path_vertex, std::string path_fragment)
        : m_name(std::move(name)),
          m_pathVertex(std::move(path_vertex)),
          m_pathFragment(std::move(path_fragment)),
          m_useSourceCode(false)
    {
    }

    IShader::IShader(std::string name, std::string vertex_source, std::string fragment_source, bool from_source)
        : m_name(std::move(name)),
          m_vertexSource(std::move(vertex_source)),
          m_fragmentSource(std::move(fragment_source)),
          m_useSourceCode(from_source)
    {
    }

    OpenGLShader::OpenGLShader(std::string name, std::string path_vertex, std::string path_fragment)
        : IShader(std::move(name), std::move(path_vertex), std::move(path_fragment))
    {
        std::string vertexCode = loadFile(m_pathVertex);
        std::string fragmentCode = loadFile(m_pathFragment);

        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

        m_programId = glCreateProgram();
        glAttachShader(m_programId, vertexShader);
        glAttachShader(m_programId, fragmentShader);
        glLinkProgram(m_programId);

        GLint success;
        glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
        if (!success)
        {
            char log[512];
            glGetProgramInfoLog(m_programId, 512, nullptr, log);
            glDeleteProgram(m_programId);
            throw std::runtime_error(std::string("Program linking failed: ") + log);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    OpenGLShader::OpenGLShader(std::string name, std::string vertex_source, std::string fragment_source, bool from_source)
        : IShader(std::move(name), std::move(vertex_source), std::move(fragment_source), from_source)
    {
        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, m_vertexSource);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, m_fragmentSource);

        m_programId = glCreateProgram();
        glAttachShader(m_programId, vertexShader);
        glAttachShader(m_programId, fragmentShader);
        glLinkProgram(m_programId);

        GLint success;
        glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
        if (!success)
        {
            char log[512];
            glGetProgramInfoLog(m_programId, 512, nullptr, log);
            glDeleteProgram(m_programId);
            throw std::runtime_error(std::string("Program linking failed: ") + log);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    std::string OpenGLShader::loadFile(const std::string &path)
    {
      
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("Failed to open shader file: " + path);

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string contents = buffer.str();

       
        if (contents.size() >= 3 &&
            static_cast<unsigned char>(contents[0]) == 0xEF &&
            static_cast<unsigned char>(contents[1]) == 0xBB &&
            static_cast<unsigned char>(contents[2]) == 0xBF)
        {
            contents.erase(0, 3);
        }

        {
            std::string normalized;
            normalized.reserve(contents.size());
            for (size_t i = 0; i < contents.size(); ++i)
            {
                unsigned char c = static_cast<unsigned char>(contents[i]);
                if (c == '\r')
                {
                    if (i + 1 < contents.size() && contents[i + 1] == '\n')
                    {
                      
                        continue;
                    }
                    normalized.push_back('\n');
                }
                else
                {
                    normalized.push_back(static_cast<char>(c));
                }
            }
            contents.swap(normalized);
        }

        auto firstNonSpace = contents.find_first_not_of("\n\t \f\v");
        if (firstNonSpace != std::string::npos && firstNonSpace > 0)
        {
            contents.erase(0, firstNonSpace);
        }

        auto firstNewline = contents.find('\n');
        if (firstNewline != std::string::npos)
        {
        }

        {
            std::string cleaned;
            cleaned.reserve(contents.size());
            for (unsigned char c : contents)
            {
                if (c == '\n' || c == '\t' || (c >= 0x20))
                {
                    cleaned.push_back(static_cast<char>(c));
                }
            }
            contents.swap(cleaned);
        }

        return contents;
    }

    GLuint OpenGLShader::compileShader(unsigned int type, const std::string &source)
    {
        GLuint shader = glCreateShader(type);
        const char *src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char log[1024] = {0};
            glGetShaderInfoLog(shader, 1024, nullptr, log);
            glDeleteShader(shader);

          
            std::string prefix = source.substr(0, 120);
            for (char &ch : prefix) { if (ch == '\n') ch = ' '; }

            std::string message = std::string("Shader compilation failed: ") + log + " | Source prefix: " + prefix;
            throw std::runtime_error(message);
        }
        return shader;
    }
    void OpenGLShader::use()
    {
        glUseProgram(m_programId);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_programId);
    }
    // void OpenGLShader::setMat4(const std::string &name, const glm::mat4 &mat)
    // {
    //     use();
    //     GLint location = glGetUniformLocation(m_programId, name.c_str());
    //     if (location != -1)
    //     {
    //         glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
    //     }
    // }
    void OpenGLShader::setUniformData(const std::string &name, const void *data, size_t size)
    {
        GLint location = glGetUniformLocation(m_programId, name.c_str());
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
