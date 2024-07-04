#version 450 core

in VS_OUT {
    vec2 texCoord;
    vec3 normal;
    vec3 fragPos;
} fs_in;

uniform vec3 c;
uniform sampler2D tex;

out vec4 Color;

void main() 
{
    Color = texture(tex, fs_in.texCoord) * vec4(c, 1.0);
}