#version 450 core
layout (points) in;
layout (triangle_strip, max_vertices = 10) out;

in VS_OUT {
	mat4 obj2proj;
    float vScale;
} geom_in[];

uniform float h_2;
uniform float r;
uniform float r_2;

out vec2 TexCoords;

struct VertexData {
    int pos_index;
    int tex_index;
};

void createTetrahedron() {
    vec4 base = gl_in[0].gl_Position;
    float s = geom_in[0].vScale;

    vec4 pos[4] = {
        geom_in[0].obj2proj * (base + vec4( 0.0, -h_2,    r, 0.0) * s),  // 0: Front
        geom_in[0].obj2proj * (base + vec4( 0.5, -h_2, -r_2, 0.0) * s),  // 1: Right
        geom_in[0].obj2proj * (base + vec4(-0.5, -h_2, -r_2, 0.0) * s),  // 2: Left
        geom_in[0].obj2proj * (base + vec4( 0.0,  h_2,  0.0, 0.0) * s)   // 3: Top
    };

    vec2 texCoords[3] = {
        vec2(0.5, 1.0),  // 0: Top
        vec2(0.0, 0.0),  // 1: Left
        vec2(1.0, 0.0)   // 2: Right
    };

    VertexData indices[10] = {
        VertexData(0,2), VertexData(1,1), VertexData(3,0),
        VertexData(2,2), VertexData(0,1), VertexData(2,0),
        VertexData(3,2), VertexData(1,1), VertexData(2,2),
        VertexData(0,0)
    };

    for (int i = 0; i < 10; ++i) {
        VertexData data = indices[i];
        gl_Position = pos[data.pos_index];
        TexCoords = texCoords[data.tex_index];
        EmitVertex();
    }
    EndPrimitive();
}

void main() {
    createTetrahedron();
}