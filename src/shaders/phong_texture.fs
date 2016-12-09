# version 420 core

struct Material {
    sampler2D    texture_diffuse1;
    sampler2D    texture_specular1;

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

uniform Material material;
uniform Light u_Light;

out vec4 oColor;

const vec3 ambientComponent(){
    return u_Light.ambientColor * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));
}

const vec3 diffuseComponent(){
    return u_Light.diffuseColor * vec3(texture(material.texture_diffuse1, fs_in.TexCoords)) * max(dot(fs_in.L, fs_in.N),0);
}

const vec3 specularComponent(){
    //vec3 reflecteDir = reflect(-fs_in.L, fs_in.V);
    vec3 H = normalize(fs_in.L + fs_in.V);
    return u_Light.specularColor * vec3(texture(material.texture_specular1, fs_in.TexCoords)) * pow(max(dot(fs_in.V, H), 0), material.shininess) ;
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