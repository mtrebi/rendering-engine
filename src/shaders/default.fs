#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
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
in vec2 vs_texture_coords;

uniform vec3 camera_position;

uniform Light light;
uniform Material material;

out vec4 color;


void main() {
  
  // Ambient
  //vec3 ambient = material.ambient * light.ambient;
  vec3 ambient = vec3(texture(material.diffuse, vs_texture_coords)) * light.ambient;

  // Diffuse 
  vec3 norm = normalize(vs_normal);
  vec3 lightDir = normalize(light.position - vs_position);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = vec3(texture(material.diffuse, vs_texture_coords)) * diff * light.diffuse;

  // Specular
  vec3 viewDir = normalize(camera_position - vs_position);
  vec3 reflectDir = normalize(reflect(-lightDir, norm));
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = vec3(texture(material.specular, vs_texture_coords)) * spec * light.specular;

  vec3 result = (ambient + diffuse + specular);
  color = vec4(result, 1.0f);
  
}