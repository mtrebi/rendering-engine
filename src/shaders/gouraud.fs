#version 330 core

in vec3 vs_color;

out vec4 color;

void main (){
    color = vec4(vs_color, 1.0f);
}

