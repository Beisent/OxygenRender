#version 330 core
in vec3 vColor;
out vec4 FragColor;

uniform vec3 uDynamicColor; 

void main()
{
    FragColor = vec4(vColor * uDynamicColor, 1.0);
}