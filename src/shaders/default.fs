#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct DirectionalLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant,
    linear,
    quadratic;

  float attenuation(vec3 p) {
    float distance = length(position - p);
    float attenuation = 1.0f / (constant + linear * distance +
      quadratic * (distance * distance));

    return attenuation;
  }
};


struct SpotLight {
  vec3 position;
  vec3 direction;
  float cutOff;
  float outerCutOff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float intensity(vec3 p) {
    vec3 L = normalize(position - p);
    vec3 D = normalize(-direction);
    float theta = dot(L, D);
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
    return intensity;
  }

};

in vec3 vs_normal;
in vec3 vs_position;
in vec2 vs_texture_coords;

uniform vec3 camera_position;

uniform DirectionalLight directional_light;
uniform PointLight point_light;
uniform SpotLight spot_light;

uniform Material material;

out vec4 color;

void main() {
  
  // Ambient
  //vec3 ambient = material.ambient * light.ambient;
  vec3 ambient = vec3(texture(material.diffuse, vs_texture_coords)) * spot_light.ambient;

  // Diffuse 
  vec3 norm = normalize(vs_normal);
  vec3 lightDir = normalize(spot_light.position - vs_position);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = vec3(texture(material.diffuse, vs_texture_coords)) * diff * spot_light.diffuse;

  // Specular
  vec3 viewDir = normalize(camera_position - vs_position);
  vec3 reflectDir = normalize(reflect(-lightDir, norm));
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = vec3(texture(material.specular, vs_texture_coords)) * spec * spot_light.specular;

  float intensity = spot_light.intensity(vs_position);

  diffuse *= intensity;
  specular *= intensity;

  vec3 result = (ambient + diffuse + specular);
  color = vec4(result, 1.0f);
  
}