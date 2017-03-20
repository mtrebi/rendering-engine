#version 330 core

in vec2 vs_texture_coords;

out vec4 fs_color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main() {
  
  fs_color = mix(
    texture(ourTexture1, vs_texture_coords),
    texture(ourTexture2, vs_texture_coords),
    0.2f
  );
  
}