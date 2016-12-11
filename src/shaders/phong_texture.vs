# version 420 core
layout (location = 0) in vec3 l_Position;
layout (location = 1) in vec3 l_Normal;
layout (location = 2) in vec2 l_TexCoords;

layout (std140, binding = 0) uniform Matrices {
    uniform mat4 Projection;
    uniform mat4 View;
};

uniform mat4 Model;

struct PointLight {  
    vec3 position;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
};

uniform PointLight pointLight;
uniform vec3 vCameraPos;

out VS_OUT {
    vec3 L;
    vec3 V;
    vec3 N;
    vec2 TexCoords;
} vs_out;

void main() {
    vec3 FragPos = vec3(Model * vec4(l_Position, 1.0f));
    vs_out.V = normalize(vCameraPos - FragPos);
    vs_out.L = normalize(pointLight.position - FragPos);
    vs_out.N = normalize(mat3(transpose(inverse(Model))) * l_Normal);
    vs_out.TexCoords = l_TexCoords;
    
    gl_Position = Projection * View * Model * vec4(l_Position, 1.0f);
}
