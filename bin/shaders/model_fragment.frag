#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// 光照属性
struct Light {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;  // 默认漫反射纹理

void main()
{
    // 从纹理获取颜色
    vec3 color = vec3(texture(texture_diffuse1, TexCoords));
    
    // 环境光
    vec3 ambient = light.ambient * color;
    
    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * color;
    
    // 镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = light.specular * spec * vec3(0.5);
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}