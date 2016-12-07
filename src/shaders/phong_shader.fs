# version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 out_Color;

uniform vec3 u_ObjColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;

uniform float u_Ka;
uniform float u_Kd;

const vec3 ambientComponent(const vec3 lightColor){
    return u_Ka * lightColor;
}

const vec3 diffuseComponent(const vec3 lightColor){
    vec3 lightDir = normalize(u_LightPos - FragPos); 
    return u_Kd * max(dot(lightDir, Normal),0) * lightColor;
}

const vec3 specularComponent(const vec3 lightColor){
    return vec3(0.0f);
}


void main(){
    
    vec3 ambient = ambientComponent(u_LightColor);
    vec3 diffuse = diffuseComponent(u_LightColor);
    vec3 specular = specularComponent(u_LightColor);

    vec3 result = (ambient + diffuse + specular) * u_ObjColor;

    out_Color = vec4(result, 1.0f);
}