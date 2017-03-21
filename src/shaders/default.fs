#version 330 core

out vec4 fs_color;

uniform vec3 object_color;
uniform vec3 light_color;

void main() {
  
  fs_color = vec4(object_color * light_color, 1.0f);
  
}