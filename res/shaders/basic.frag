#version 430 core
in vec2 TexCoords;

uniform vec3 c;
uniform sampler2D tex;

out vec4 Color;

void main() {
    float gamma = 2.2;
    Color = vec4(pow(texture(tex, TexCoords).rgb * c, vec3(1.0/gamma)), 1.0);
}