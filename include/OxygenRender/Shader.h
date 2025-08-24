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

        static std::string loadFile(const std::string &path);
        static GLuint compileShader(GLenum type, const std::string &source);

    public:
        OpenGLShader(std::string name, std::string path_vertex, std::string path_fragment);
        virtual ~OpenGLShader() override;
        virtual void use() override;
        inline virtual GLuint getID() override { return m_program_id; }

        virtual void setUniformData(const std::string &name, const void *data, size_t size) override;
        void setMat4(const std::string &name, const glm::mat4 &mat);
    };

    class ShaderFactory
    {
    public:
        static std::unique_ptr<IShader> create(std::string name, std::string path_vertex, std::string path_fragment);
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

        inline GLuint getID() { return m_Shader->getID(); }
        inline void use() { m_Shader->use(); }
        inline void setUniformData(const std::string &name, const void *data, size_t size) { m_Shader->setUniformData(name, data, size); }
    };
};
