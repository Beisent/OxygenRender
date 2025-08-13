#pragma once
#include <string>
namespace OxyRender
{
    class IShader
    {
    private:
        std::string m_name;
        std::string m_path_vertex;
        std::string m_path_fragment;

    public:
        IShader(std::string name, std::string path_vertex, std::string path_fragment);
        virtual ~IShader() = default;
    };
};