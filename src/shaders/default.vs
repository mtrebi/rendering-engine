#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vs_position;
out vec3 vs_normal;
out vec2 vs_texture_coords;

void main() {
  gl_Position = projection * view * model * vec4(position, 1.0f);
  vs_position = vec3(model * vec4(position, 1.0f));
  vs_normal = mat3(transpose(inverse(model))) * normal;
  vs_texture_coords = texture_coords;
}