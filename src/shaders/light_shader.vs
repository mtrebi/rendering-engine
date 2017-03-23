#version 420 core

layout(location = 0) in vec3 position;

layout(std140, binding = 0) uniform Matrices {
  mat4 view;
  mat4 projection;
};

uniform mat4 model;

void main() {
  gl_Position = projection * view * model * vec4(position, 1.0f);
}