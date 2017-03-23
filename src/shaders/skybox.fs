#version 420 core

in VS_OUT{
  vec3 texture_coords;
} fs_in;

out vec4 color;

uniform samplerCube skybox;

void main()
{
  color = texture(skybox, fs_in.texture_coords);
}
