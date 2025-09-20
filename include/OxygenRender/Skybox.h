#pragma once
#include "OxygenRender/Renderer.h"
#include "OxygenRender/Shader.h"
#include "OxygenRender/Texture.h"
#include "OxygenRender/Buffer.h"
#include <vector>
#include <glm/glm.hpp>  

namespace OxyRender {

class Skybox {
public:
    Skybox(Renderer& renderer, const std::vector<std::string>& faces);
    ~Skybox();
    
    void draw(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    static Shader createDefaultShader();
    
private:
    Renderer& m_renderer;
    Cubemap m_cubemap;
    VertexArray m_vao;
    Buffer m_vbo;
    
    void setupMesh();
    static const char* s_vertexShaderSrc;
    static const char* s_fragmentShaderSrc;
};

} // namespace OxyRender