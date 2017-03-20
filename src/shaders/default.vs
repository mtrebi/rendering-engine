#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texture_coords;

out vec3 vs_color;
out vec2 vs_texture_coords;

void main()
{
  gl_Position = vec4(position.x, position.y, position.z, 1.0);
  vs_color = color;
  vs_texture_coords = vec2(texture_coords.x, 1.0f - texture_coords.y);
}