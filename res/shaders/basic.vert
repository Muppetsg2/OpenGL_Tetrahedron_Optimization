#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in float scale;

uniform mat4 view;
uniform mat4 projection;

out float vScale;

void main() {
    gl_Position = projection * view * vec4(position, 1.0);
    vScale = scale;
}