#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texture_coords;


uniform mat4 transform;

out vec3 vs_color;
out vec2 vs_texture_coords;

void main() {
  gl_Position = transform * vec4(position, 1.0f);
  vs_color = color;
  vs_texture_coords = vec2(texture_coords.x, 1.0f - texture_coords.y);
}