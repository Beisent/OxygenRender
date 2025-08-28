#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aNormal;

out vec3 vFragPos;
out vec3 vNormal;
out vec4 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    vFragPos = worldPos.xyz;
    
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    
    vColor = aColor;
    
    gl_Position = projection * view * worldPos;
}
