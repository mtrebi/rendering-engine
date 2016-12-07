#version 330 core
layout(location = 0) in vec3 l_Position;
layout(location = 1) in vec3 l_Normal;

struct Material {
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;

    float shininess;
};

struct Light {
    vec3 position;

    Material material;
};

uniform vec3 u_CameraPos;
uniform Material u_Material;
uniform Light u_Light;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 vs_color;

const vec3 ambientComponent(){
    return u_Light.material.ambientColor * u_Material.ambientColor;
}

const vec3 diffuseComponent(){
    vec3 lightDir = normalize(u_Light.position - l_Position); 
    return u_Light.material.diffuseColor * u_Material.diffuseColor * max(dot(lightDir, l_Normal),0);
}

const vec3 specularComponent(){
    vec3 lightDir = normalize(u_Light.position - l_Position); 
    vec3 viewDir = normalize(u_CameraPos - l_Position);
    vec3 reflecteDir = reflect(-lightDir ,l_Normal);
    return u_Light.material.specularColor * u_Material.specularColor * pow(max(dot(viewDir, reflecteDir), 0), u_Material.shininess) ;
}

const vec3 phongShading(){
    vec3 ambient = ambientComponent();
    vec3 diffuse = diffuseComponent();
    vec3 specular = specularComponent();

    return (ambient + diffuse + specular);
}

void main(){
    gl_Position = u_Projection * u_View * u_Model * vec4(l_Position, 1.0f);
    vs_color = phongShading();
}