#version 330 core

in vec3 vs_color;

out vec4 fs_color;

void main()
{
  fs_color = vec4(vs_color.xyz, 1.0f);
}