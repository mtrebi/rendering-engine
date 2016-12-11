# version 420 core

struct Material {
    sampler2D    texture_diffuse1;
    sampler2D    texture_specular1;

    float shininess;
};

struct PointLight {  
    vec3 position;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;

    float Kc;
    float Kl;
    float Kq;
};

struct DirectionalLight {  
    vec3 direction;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
};

in VS_OUT {
    vec3 FragPosition;
    vec3 V;
    vec3 N;
    vec2 TexCoords;
} fs_in;

uniform Material material;
uniform PointLight pointLight;
uniform DirectionalLight directionalLight;

out vec4 oColor;

const vec3 ambientComponent(const vec3 Ka){
    return Ka * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));
}

const vec3 diffuseComponent(const vec3 Kd, const vec3 L){
    return Kd * vec3(texture(material.texture_diffuse1, fs_in.TexCoords)) * max(dot(L, fs_in.N),0);
}

const vec3 specularComponent(const vec3 Ks, const vec3 L){
    //vec3 reflecteDir = reflect(-fs_in.L, fs_in.V);
    vec3 H = normalize(L + fs_in.V);
    return Ks * vec3(texture(material.texture_specular1, fs_in.TexCoords)) * pow(max(dot(fs_in.N, H), 0), material.shininess) ;
}

const vec3 phongShading(const vec3 Ka, const vec3 Kd, const vec3 Ks, const vec3 L){
    vec3 ambient = ambientComponent(Ka);
    vec3 diffuse = diffuseComponent(Kd, L);
    vec3 specular = specularComponent(Ks, L);

    return (ambient + diffuse + specular);
}

const vec3 directionalContribution(){   
    return phongShading(directionalLight.Ka, directionalLight.Kd, directionalLight.Ks, normalize(-directionalLight.direction));
}

const float lightAttenuation(const float Kc, const float Kl, const float Kq, const float distance){
    return 1 / (Kc + Kl * distance + Kq * pow(distance, 2));
}

const vec3 pointLightContribution(const PointLight pointLight){
    const vec3 lightDirection = normalize(pointLight.position - fs_in.FragPosition);
    const vec3 phong = phongShading(pointLight.Ka, pointLight.Kd, pointLight.Ks, lightDirection);
    const float distance = length(pointLight.position - fs_in.FragPosition);
    const float attenuation = lightAttenuation(pointLight.Kc, pointLight.Kl, pointLight.Kq, distance);
    const vec3 result = phong * attenuation;

    return result;
}

void main(){
    const vec3 dirContr = directionalContribution();
    vec3 pointContr = pointLightContribution(pointLight);
    
    const vec3 result = dirContr + pointContr;
    oColor = vec4(result, 1.0f);
    //oColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}