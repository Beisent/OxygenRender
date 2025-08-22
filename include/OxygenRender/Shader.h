#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <glad/glad.h>
#include <memory>
#include <glm/glm.hpp> 
#include "OxygenRender/GraphicsTypes.h"
namespace OxyRender
{
    class IShader
    {
    protected:
        std::string m_name;
        std::string m_path_vertex;
        std::string m_path_fragment;

    public:
        IShader(std::string name, std::string path_vertex, std::string path_fragment);
        virtual void use() = 0;
        virtual void setUniformData(const std::string &name, const void *data, size_t size) = 0;
        virtual GLuint getID() = 0;
        virtual ~IShader() = default;
    };

    class OpenGLShader : public IShader
    {
    private:
        GLuint m_program_id;

        static std::string loadFile(const std::string &path)
        {
            std::ifstream file(path);
            if (!file.is_open())
                throw std::runtime_error("Failed to open shader file: " + path);

            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }

        static GLuint compileShader(GLenum type, const std::string &source)
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

    public:
        OpenGLShader(std::string name, std::string path_vertex, std::string path_fragment);
        virtual ~OpenGLShader() override;
        virtual void use() override;
        virtual GLuint getID() override;
        virtual void setUniformData(const std::string &name, const void *data, size_t size) override;
        void setMat4(const std::string &name, const glm::mat4 &mat)
        {
            use();
            GLint location = glGetUniformLocation(m_program_id, name.c_str());
            if (location != -1)
            {
                glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
            }
        }
    };

    class ShaderFactory
    {
    public:
        static std::unique_ptr<IShader> create(std::string name, std::string path_vertex, std::string path_fragment)
        {
            switch (Backends::OXYG_CurrentBackend)
            {
            case RendererBackend::OpenGL:
                return std::make_unique<OpenGLShader>(name, path_vertex, path_fragment);
            default:
                return nullptr;
            }
        }
    };
    class Shader
    {
    private:
        std::unique_ptr<IShader> m_Shader;

    public:
        Shader(std::string name, std::string path_vertex, std::string path_fragment)
        {
            m_Shader = ShaderFactory::create(name, path_vertex, path_fragment);
        }
        ~Shader() = default;

        GLuint getID()
        {
            return m_Shader->getID();
        }
        void use()
        {
            m_Shader->use();
        }
        void setUniformData(const std::string &name, const void *data, size_t size)
        {
            m_Shader->setUniformData(name, data, size);
        }
        
    };
};
