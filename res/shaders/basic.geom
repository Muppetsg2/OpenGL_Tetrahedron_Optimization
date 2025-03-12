#version 450 core
layout (points) in;
layout (triangle_strip, max_vertices = 13) out;

in VS_OUT {
	mat4 obj2proj;
    float vScale;
} geom_in[];

out vec2 TexCoords;

void createTetrahedron() {
    float sqrt_3 = sqrt(3.0);
    float h_2 = 1.0 / 3.0;
    float r = sqrt_3 / 3.0;
    float r_2 = sqrt_3 / 6.0;
    float half_1 = 0.5;

    /* IF Triangles not triangle_strip
    vec3 offsets[4] = {
        vec3(0.0, -h_2, r),         // Front
        vec3(half_1, -h_2, -r_2),   // Right
        vec3(-half_1, -h_2, -r_2),  // Left
        vec3(0.0, h_2, 0.0)         // Top
    };

    vec2 texCoords[6] = {
        vec2(0.5, 1.0), vec2(0.0, 0.0), 
        vec2(1.0, 0.0), vec2(0.0, 1.0), 
        vec2(1.0, 1.0), vec2(0.5, 0.0)
    };

    int off_indices[12] = {
        0, 2, 1,
        0, 1, 3,
        1, 2, 3,
        2, 0, 3
    };

    int tex_indices[12] = {
        0, 2, 1,
        3, 4, 5,
        3, 4, 5,
        3, 4, 5
    };
    */

    vec3 offsets[4] = {
        vec3(0.0, -h_2, r),         // Front
        vec3(half_1, -h_2, -r_2),   // Right
        vec3(-half_1, -h_2, -r_2),  // Left
        vec3(0.0, h_2, 0.0)         // Top
    };

    vec2 texCoords[6] = {
        vec2(0.5, 1.0), vec2(0.0, 0.0), 
        vec2(1.0, 0.0), vec2(0.0, 1.0), 
        vec2(1.0, 1.0), vec2(0.5, 0.0)
    };

    int off_indices[13] = {
        0, 1, 3, 2, 0, 2, 3, 1, 2, 1, 0, 2, 1
    };

    int tex_indices[13] = {
        3, 4, 5, 3, 4, 3, 5, 3, 2, 1, 0, 2, 1
    };

    for (int i = 0; i < 13; ++i) {
        gl_Position = geom_in[0].obj2proj * (gl_in[0].gl_Position + vec4(offsets[off_indices[i]] * geom_in[0].vScale, 0.0));
        TexCoords = texCoords[tex_indices[i]];
        EmitVertex();
    }
    EndPrimitive();
}

void main() {
    createTetrahedron();
}