# version 330 core

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


in vec3 FragPos;
in vec3 Normal;


uniform vec3 u_CameraPos;
uniform Material u_Material;
uniform Light u_Light;

out vec4 out_Color;


const vec3 ambientComponent(){
    return u_Light.material.ambientColor * u_Material.ambientColor;
}

const vec3 diffuseComponent(){
    vec3 lightDir = normalize(u_Light.position - FragPos); 
    return u_Light.material.diffuseColor * u_Material.diffuseColor * max(dot(lightDir, Normal),0);
}

const vec3 specularComponent(){
    vec3 lightDir = normalize(u_Light.position - FragPos); 
    vec3 viewDir = normalize(u_CameraPos - FragPos);
    vec3 reflecteDir = reflect(-lightDir ,Normal);
    return u_Light.material.specularColor * u_Material.specularColor * pow(max(dot(viewDir, reflecteDir), 0), u_Material.shininess) ;
}

const vec3 phongShading(){
    vec3 ambient = ambientComponent();
    vec3 diffuse = diffuseComponent();
    vec3 specular = specularComponent();

    return (ambient + diffuse + specular);
}

void main(){
    out_Color = vec4(phongShading(), 1.0f);
}