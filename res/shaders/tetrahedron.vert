#version 450 core  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float t;

void main()  
{  
  gl_Position = projection * view * model * vec4(position, 1.f);
  //TexCoord = vec2(texCoord.x - t, texCoord.y);
  TexCoord = texCoord;
}