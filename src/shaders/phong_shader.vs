# version 330 core
layout (location = 0) in vec3 l_Position;
layout (location = 1) in vec3 l_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 FragPos;
out vec3 Normal;

void main() {
    gl_Position = u_Projection * u_View * u_Model * vec4(l_Position, 1.0f);
    FragPos = vec3(u_Model * vec4(l_Position, 1.0f));
    Normal = l_Normal;    
}