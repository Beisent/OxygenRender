#version 330 core
in vec4 vColor; // 从顶点着色器传入
out vec4 FragColor;

void main()
{
    FragColor = vColor;
}
