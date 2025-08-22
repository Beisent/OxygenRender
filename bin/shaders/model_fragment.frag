#version 330 core

out vec4 FragColor;

in vec3 FragPos;

in vec2 TexCoords;

in mat3 TBN;

struct Light {

  vec3 position;

  vec3 ambient;

  vec3 diffuse;

  vec3 specular;

};

uniform Light light;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;

uniform sampler2D texture_specular1;

uniform sampler2D texture_normal1;

void main()

{

  

  vec3 color = texture(texture_diffuse1, TexCoords).rgb;

  vec3 specularColor = texture(texture_specular1, TexCoords).rgb;

  

  vec3 tangentNormal = texture(texture_normal1, TexCoords).rgb;

  tangentNormal = tangentNormal * 2.0 - 1.0;

  

  vec3 norm = normalize(TBN * tangentNormal);

  vec3 lightDir = normalize(light.position - FragPos);

  vec3 viewDir  = normalize(viewPos - FragPos);

  vec3 reflectDir = reflect(-lightDir, norm);

  vec3 ambient = light.ambient * color;

  float diff = max(dot(norm, lightDir), 0.0);

  vec3 diffuse = light.diffuse * diff * color;

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

  vec3 specular = light.specular * spec * specularColor;

  vec3 result = ambient + diffuse + specular;

  FragColor = vec4(result, 1.0);

}