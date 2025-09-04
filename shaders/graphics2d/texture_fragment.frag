#version 330 core
in vec4 vColor;
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform bool uUseTexture;

void main()
{
    if (uUseTexture)
    {
        FragColor = texture(uTexture, vTexCoord) * vColor;
    }
    else
    {
        FragColor = vColor;
    }
}
