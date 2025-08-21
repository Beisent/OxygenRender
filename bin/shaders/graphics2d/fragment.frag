#version 330 core
in vec4 vColor;
in vec2 vLocalPos;
out vec4 FragColor;

uniform vec2 uCenter;     
uniform float uRadius;    
uniform float uThickness; 

void main()
{
    float dist = length(vLocalPos - uCenter);

   
    float inner = uRadius - uThickness * 0.5;
    float outer = uRadius + uThickness * 0.5;

    
    float alpha = smoothstep(inner - 1.0, inner + 1.0, dist) *
                  (1.0 - smoothstep(outer - 1.0, outer + 1.0, dist));

    FragColor = vec4(vColor.rgb, vColor.a * alpha);
}
