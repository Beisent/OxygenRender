#include "OxygenRender/Shader.h"
namespace OxyRender
{
    IShader::IShader(std::string name, std::string path_vertex, std::string path_fragment)
        : m_name(name), m_path_vertex(path_vertex), m_path_fragment(path_fragment)
    {
    }
}