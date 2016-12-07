# version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 out_Color;

uniform vec3 u_ObjColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;

uniform float u_Ka;
uniform float u_Kd;
uniform float u_Ks;
uniform float u_shininess;

const vec3 ambientComponent(const vec3 lightColor){
    return lightColor * u_Ka;
}

const vec3 diffuseComponent(const vec3 lightColor){
    vec3 lightDir = normalize(u_LightPos - FragPos); 
    return lightColor * u_Kd * max(dot(lightDir, Normal),0);
}

const vec3 specularComponent(const vec3 lightColor){
    vec3 lightDir = normalize(u_LightPos - FragPos); 
    vec3 viewDir = normalize(u_CameraPos - FragPos);
    vec3 reflecteDir = reflect(-lightDir ,Normal);
    return lightColor * u_Ks * pow(max(dot(viewDir, reflecteDir), 0), u_shininess) ;
}


void main(){
    
    vec3 ambient = ambientComponent(u_LightColor);
    vec3 diffuse = diffuseComponent(u_LightColor);
    vec3 specular = specularComponent(u_LightColor);

    vec3 result = (ambient + diffuse + specular) * u_ObjColor;

    out_Color = vec4(result, 1.0f);
}