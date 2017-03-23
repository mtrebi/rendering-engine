#version 420 core
layout(location = 0) in vec3 position;

//TODO: Use UBOs
uniform mat4 view;
uniform mat4 projection;


out VS_OUT{
  vec3 texture_coords;
} vs_out;

void main()
{
  vec4 pos = projection * view * vec4(position, 1.0);
  gl_Position = pos.xyww;
  vs_out.texture_coords = position;
}