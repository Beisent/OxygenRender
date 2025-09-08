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
        // 从源码或文件自动判别加载（两参数重载）
        Shader(std::string name, const char* v, const char* f)
        {
            auto looks_like_source = [](const char* s) -> bool {
                if (s == nullptr) return false;
                // 跳过前导空白
                const char* p = s;
                while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n' || *p == '\f' || *p == '\v') ++p;
                // 典型GLSL开头
                if (p[0] == '#' && p[1] == 'v') return true; // #version
                // 在前64字符内出现GLSL特征
                int scanned = 0;
                for (const char* q = p; *q && scanned < 256; ++q, ++scanned) {
                    if (*q == '\n' || *q == '\r') return true; // 多行，即源码
                    if ((q[0] == 'l' && q[1] == 'a' && q[2] == 'y' && q[3] == 'o' && q[4] == 'u' && q[5] == 't') ||
                        (q[0] == 'u' && q[1] == 'n' && q[2] == 'i' && q[3] == 'f' && q[4] == 'o' && q[5] == 'r' && q[6] == 'm') ||
                        (q[0] == 'v' && q[1] == 'o' && q[2] == 'i' && q[3] == 'd' && (q[4] == ' ' || q[4] == '\t')))
                        return true;
                }
                return false;
            };

            const bool v_is_src = looks_like_source(v);
            const bool f_is_src = looks_like_source(f);

            if (v_is_src || f_is_src) {
                m_Shader = ShaderFactory::createFromSource(name, std::string(v ? v : ""), std::string(f ? f : ""));
            } else {
                m_Shader = ShaderFactory::create(name, std::string(v ? v : ""), std::string(f ? f : ""));
            }
        }
        // 禁用拷贝，启用移动
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&&) noexcept = default;
        Shader& operator=(Shader&&) noexcept = default;
        ~Shader() = default;

        inline GLuint getID() { return m_Shader->getID(); }
        inline void use() { m_Shader->use(); }
        inline void setUniformData(const std::string &name, const void *data, size_t size) { m_Shader->setUniformData(name, data, size); }
    };
};
