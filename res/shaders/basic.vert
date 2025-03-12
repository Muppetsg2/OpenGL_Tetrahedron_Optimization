#version 450 core
layout (location = 0) in vec3 point;
layout (location = 1) in vec3 position;
layout (location = 2) in float scale;

uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
	mat4 obj2proj;
    float vScale;
} vs_out;

void main() {
    vs_out.obj2proj = projection * view * mat4(1.0);
    gl_Position = vec4(position, 1.0);
    vs_out.vScale = scale;
}