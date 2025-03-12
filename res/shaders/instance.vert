#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in mat4 instanceMatrix;

uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
	vec2 texCoord;
} vs_out;

void main()  
{  
  gl_Position = projection * view * instanceMatrix * vec4(position, 1.0);
  vs_out.texCoord = texCoord;
}