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
    // 渲染器抽象接口类
    class IShader
    {
    protected:
        std::string m_name;
        std::string m_path_vertex;
        std::string m_path_fragment;
        std::string m_vertex_source;
        std::string m_fragment_source;
        bool m_use_source_code;

    public:
        IShader(std::string name, std::string path_vertex, std::string path_fragment);
        IShader(std::string name, std::string vertex_source, std::string fragment_source, bool from_source);
        virtual void use() = 0;
        virtual void setUniformData(const std::string &name, const void *data, size_t size) = 0;
        virtual GLuint getID() = 0;
        virtual ~IShader() = default;
    };
    // OpenGL实现Shader
    class OpenGLShader : public IShader
    {
    private:
        GLuint m_program_id;

        static std::string loadFile(const std::string &path);
        static GLuint compileShader(GLenum type, const std::string &source);

    public:
        OpenGLShader(std::string name, std::string path_vertex, std::string path_fragment);
        OpenGLShader(std::string name, std::string vertex_source, std::string fragment_source, bool from_source);
        virtual ~OpenGLShader() override;
        virtual void use() override;
        inline virtual GLuint getID() override { return m_program_id; }

        virtual void setUniformData(const std::string &name, const void *data, size_t size) override;
        void setMat4(const std::string &name, const glm::mat4 &mat);
    };
    // 渲染器工厂类
    class ShaderFactory
    {
    public:
        static std::unique_ptr<IShader> create(std::string name, std::string path_vertex, std::string path_fragment);
        static std::unique_ptr<IShader> createFromSource(std::string name, std::string vertex_source, std::string fragment_source);
    };

    // Shader类对外接口
    class Shader
    {
    private:
        std::unique_ptr<IShader> m_Shader;

    public:
        // 从文件加载着色器
        Shader(std::string name, std::string path_vertex, std::string path_fragment)
        {
            m_Shader = ShaderFactory::create(name, path_vertex, path_fragment);
        }
        // 从源码编译着色器
        Shader(std::string name, const char* vertex_source, const char* fragment_source)
        {
            m_Shader = ShaderFactory::createFromSource(name, std::string(vertex_source), std::string(fragment_source));
        }
        ~Shader() = default;

        inline GLuint getID() { return m_Shader->getID(); }
        inline void use() { m_Shader->use(); }
        inline void setUniformData(const std::string &name, const void *data, size_t size) { m_Shader->setUniformData(name, data, size); }
    };
};
