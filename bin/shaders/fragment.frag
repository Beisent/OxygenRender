#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;

void main()
{
    
    vec4 texColor = texture(uTexture, TexCoord);
    if(TexCoord == vec2(0.0)) 
        FragColor = vec4(ourColor, 1.0);
    else
        FragColor = texColor * vec4(ourColor, 1.0);
}
