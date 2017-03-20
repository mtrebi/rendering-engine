#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec2 vs_texture_coords;

void main() {
  gl_Position = projection * view * model * vec4(position, 1.0f);
  vs_texture_coords = vec2(texture_coords.x, 1.0f - texture_coords.y);
}