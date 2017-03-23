#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out VS_OUT {
  vec3 normal;
  vec3 position;
  vec2 texture_coords;
} vs_out;


void main() {
  gl_Position = projection * view * model * vec4(position, 1.0f);
  vs_out.position = vec3(model * vec4(position, 1.0f));
  vs_out.normal = mat3(transpose(inverse(model))) * normal;
  vs_out.texture_coords = texture_coords;
}