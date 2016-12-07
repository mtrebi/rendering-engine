# version 330 core

struct Material {
    sampler2D    diffuseTexture;
    sampler2D    specularTexture;

    float shininess;
};

struct Light {
    vec3 position;
    
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
};


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 u_CameraPos;
uniform Material u_Material;
uniform Light u_Light;

out vec4 out_Color;


const vec3 ambientComponent(){
    return u_Light.ambientColor * vec3(texture(u_Material.diffuseTexture, TexCoords));
}

const vec3 diffuseComponent(){
    vec3 lightDir = normalize(u_Light.position - FragPos); 
    return u_Light.diffuseColor * vec3(texture(u_Material.diffuseTexture, TexCoords)) * max(dot(lightDir, Normal),0);
}

const vec3 specularComponent(){
    vec3 lightDir = normalize(u_Light.position - FragPos); 
    vec3 viewDir = normalize(u_CameraPos - FragPos);
    vec3 reflecteDir = reflect(-lightDir ,Normal);
    return u_Light.specularColor * vec3(texture(u_Material.specularTexture, TexCoords)) * pow(max(dot(viewDir, reflecteDir), 0), u_Material.shininess) ;
}

const vec3 phongShading(){
    vec3 ambient = ambientComponent();
    vec3 diffuse = diffuseComponent();
    vec3 specular = specularComponent();

    return (ambient + diffuse + specular);
}

void main(){
    //out_Color = vec4(texture(u_Material.diffuseTexture, TexCoords));
    out_Color = vec4(phongShading(), 1.0f);
}