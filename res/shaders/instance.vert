#version 450 core  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in mat4 instanceMatrix;

uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
	vec2 texCoord;
	vec3 normal;
	vec3 fragPos;
} vs_out;

void main()  
{  
  gl_Position = projection * view * instanceMatrix * vec4(position, 1.0);
  vs_out.texCoord = texCoord;
  vs_out.normal = normal;
  vs_out.fragPos = vec3(instanceMatrix * vec4(position, 1.0));
}