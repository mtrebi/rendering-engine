# version 420 core

struct Material {
    sampler2D    diffuseTexture;
    sampler2D    specularTexture;

    float shininess;
};

struct Light {    
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
};

in VS_OUT {
    vec3 L;
    vec3 V;
    vec3 N;
    vec2 TexCoords;
} fs_in;

uniform Material u_Material;
uniform Light u_Light;

out vec4 oColor;

const vec3 ambientComponent(){
    return u_Light.ambientColor * vec3(texture(u_Material.diffuseTexture, fs_in.TexCoords));
}

const vec3 diffuseComponent(){
    return u_Light.diffuseColor * vec3(texture(u_Material.diffuseTexture, fs_in.TexCoords)) * max(dot(fs_in.L, fs_in.N),0);
}

const vec3 specularComponent(){
    //vec3 reflecteDir = reflect(-fs_in.L, fs_in.V);
    vec3 H = normalize(fs_in.L + fs_in.V);
    return u_Light.specularColor * vec3(texture(u_Material.specularTexture, fs_in.TexCoords)) * pow(max(dot(fs_in.V, H), 0), u_Material.shininess) ;
}

const vec3 phongShading(){
    vec3 ambient = ambientComponent();
    vec3 diffuse = diffuseComponent();
    vec3 specular = specularComponent();

    return (ambient + diffuse + specular);
}

void main(){
    oColor = vec4(phongShading(), 1.0f);
}