# version 330 core

out vec4 out_Color;


uniform vec3 u_ObjColor;
uniform vec3 u_LightColor;

uniform float u_Ka;

const vec3 ambientComponent(const vec3 lightColor){
    return 0.2 * lightColor;
}

const vec3 diffuseComponent(const vec3 lightColor){
    //return u_Ka * color;
    return vec3(0.0f);
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