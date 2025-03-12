#version 450 core
layout (points) in;
layout (triangle_strip, max_vertices = 12) out;

in float vScale[];
out vec2 TexCoords;

void createTetrahedron() {
    float sqrt_3 = sqrt(3.0);
    float h_2 = 1.0 / 3.0;
    float r = sqrt_3 / 3.0;
    float r_2 = sqrt_3 / 6.0;
    float half_1 = 0.5;

    vec3 offsets[10] = {
        vec3(0.0, -h_2, r), vec3(half_1, -h_2, -r_2),
        vec3(-half_1, -h_2, -r_2), vec3(0.0, -h_2, r),
        vec3(half_1, -h_2, -r_2), vec3(0.0, h_2, 0.0),
        vec3(half_1, -h_2, -r_2), vec3(-half_1, -h_2, -r_2),
        vec3(-half_1, -h_2, -r_2), vec3(0.0, -h_2, r)
    };

    vec2 texCoords[10] = {
        vec2(0.5, 1.0), vec2(0.0, 0.0), 
        vec2(1.0, 0.0), vec2(0.0, 1.0), 
        vec2(1.0, 1.0), vec2(0.5, 0.0),
        vec2(0.0, 1.0), vec2(1.0, 1.0), 
        vec2(0.0, 1.0), vec2(1.0, 1.0)
    };

    int indices[12] = {
        0, 2, 1,
        3, 4, 5,
        6, 7, 5,
        8, 9, 5
    };

    for (int i = 0; i < 12; ++i) {
        gl_Position = gl_in[0].gl_Position + vec4(offsets[indices[i]] * vScale[0], 0.0);
        TexCoords = texCoords[indices[i]];
        EmitVertex();
    }
    EndPrimitive();
}

void main() {
    createTetrahedron();
}