# version 330 core

layout (location = 0) in vec3 iPosition;

void main() {
    gl_Position = vec4(iPosition.x, iPosition.y, iPosition.z, 1);
}