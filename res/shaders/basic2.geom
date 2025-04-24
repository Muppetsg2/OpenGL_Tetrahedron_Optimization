#version 450 core
layout (points) in;
layout (triangle_strip, max_vertices = 9) out;

in VS_OUT {
    mat4 obj2proj;
    float vScale;
} geom_in[];

uniform float h_2;
uniform float r;
uniform float r_2;

out vec2 TexCoords;

struct VertexData {
    int a, b, c;
    int ta, tb, tc;
};

void EmitTriangle(vec4 a, vec4 b, vec4 c, vec2 ta, vec2 tb, vec2 tc) {
    vec3 ab = b.xyz - a.xyz;
    vec3 ac = c.xyz - a.xyz;
    vec3 normal = normalize(cross(ab, ac));

    if (dot(normal, vec3(0, 0, -1)) > 0.0)
        return;

    gl_Position = a; TexCoords = ta; EmitVertex();
    gl_Position = b; TexCoords = tb; EmitVertex();
    gl_Position = c; TexCoords = tc; EmitVertex();
    EndPrimitive();
}

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
        vec2(0.5, 1.0),  // Top
        vec2(0.0, 0.0),  // Left
        vec2(1.0, 0.0)   // Right
    };

    EmitTriangle(pos[0], pos[1], pos[3], texCoords[2], texCoords[1], texCoords[0]); // Front
    EmitTriangle(pos[2], pos[0], pos[3], texCoords[2], texCoords[1], texCoords[0]); // Left
    EmitTriangle(pos[1], pos[2], pos[3], texCoords[2], texCoords[1], texCoords[0]); // Right
    EmitTriangle(pos[0], pos[2], pos[1], texCoords[0], texCoords[1], texCoords[2]); // Bottom
}

void main() {
    createTetrahedron();
}