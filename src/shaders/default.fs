#version 330 core

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};


in vec3 vs_normal;
in vec3 vs_position;

uniform vec3 camera_position;

uniform Light light;
uniform Material material;

out vec4 color;


void main() {
  
  // Ambient
  vec3 ambient = material.ambient * light.ambient;

  // Diffuse 
  vec3 norm = normalize(vs_normal);
  vec3 lightDir = normalize(light.position - vs_position);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = material.diffuse * diff * light.diffuse;

  // Specular
  vec3 viewDir = normalize(camera_position - vs_position);
  vec3 reflectDir = normalize(reflect(-lightDir, norm));
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = material.specular * spec * light.specular;

  vec3 result = (ambient + diffuse + specular);
  color = vec4(result, 1.0f);
  
}