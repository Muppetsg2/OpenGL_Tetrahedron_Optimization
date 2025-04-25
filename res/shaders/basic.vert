#version 450 core
layout (location = 0) in vec3 point;

struct InstanceData {
    vec3 pos;
    float scale;
};

layout(std430, binding = 0) buffer InstanceBuffer {
    InstanceData instances[];
};

uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
	mat4 obj2proj;
    float vScale;
} vs_out;

void main() {
    InstanceData data = instances[gl_InstanceID];
    vs_out.obj2proj = projection * view * mat4(1.0);
    gl_Position = vec4(data.pos, 1.0);
    vs_out.vScale = data.scale;
}