#version 450 core  
in vec2 TexCoord;

out vec4 color;

uniform vec3 c;
uniform sampler2D tex;

void main()  
{  
    color = texture(tex, TexCoord) * vec4(c, 1.0);
}