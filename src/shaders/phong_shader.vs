# version 330 core
layout (location = 0) in vec3 in_Position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
    gl_Position = u_Projection * u_View * u_Model * vec4(in_Position, 1.0f);
}