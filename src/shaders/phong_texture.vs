# version 330 core
layout (location = 0) in vec3 l_Position;
layout (location = 1) in vec3 l_Normal;
layout (location = 2) in vec2 l_TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main() {
    gl_Position = u_Projection * u_View * u_Model * vec4(l_Position, 1.0f);
    FragPos = vec3(u_Model * vec4(l_Position, 1.0f));
    Normal = mat3(transpose(inverse(u_Model))) * l_Normal;
    TexCoords = l_TexCoords;
}
