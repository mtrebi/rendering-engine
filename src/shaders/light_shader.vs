# version 420 core
layout (location = 0) in vec3 l_Position;

layout (std140, binding = 0) uniform Matrices {
    mat4 Projection;
    mat4 View;
};

uniform mat4 Model;

void main() {
    gl_Position = Projection * View * Model * vec4(l_Position, 1.0f);
}