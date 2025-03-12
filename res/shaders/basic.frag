#version 450 core
in vec2 TexCoords;

uniform vec3 c;
uniform sampler2D tex;

out vec4 Color;

void main() {
    Color = texture(tex, TexCoords) * vec4(c, 1.0);
}