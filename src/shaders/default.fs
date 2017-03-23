#version 330 core

struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
  sampler2D texture_reflection1;

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

vec3 CalcPhong(vec3 light_ambient, vec3 light_diffuse, vec3 light_specular, vec3 N, vec3 L, vec3 V);
vec3 CalcDirLight(DirectionalLight light, vec3 N, vec3 V);
vec3 CalcPointLight(PointLight light, vec3 N, vec3 position, vec3 V);
vec3 CalcSpotLight(SpotLight light, vec3 N, vec3 position, vec3 V);

void main() {
  vec3 N = normalize(vs_normal);
  vec3 V = normalize(camera_position - vs_position);
  vec3 result;
  result += CalcDirLight(directional_light, N, V);
  result += CalcPointLight(point_light, N, vs_position, V);
  result += CalcSpotLight(spot_light, N, vs_position, V);

  //color = vec4(vec3(texture(material.texture_diffuse1, vs_texture_coords)), 1.0f);
  color = vec4(result, 1.0f);
}


vec3 CalcPhong(vec3 light_ambient, vec3 light_diffuse, vec3 light_specular, vec3 N, vec3 L, vec3 V) {  
  vec3 A = vec3(texture(material.texture_diffuse1, vs_texture_coords)) * light_ambient;

  // Diffuse 
  float diff = max(dot(N, L), 0.0);
  vec3 D = vec3(texture(material.texture_diffuse1, vs_texture_coords)) * diff * light_diffuse;

  // Specular
  vec3 R = normalize(reflect(-L, N));
  float spec = pow(max(dot(V, R), 0.0), material.shininess);
  vec3 S = vec3(texture(material.texture_specular1, vs_texture_coords)) * spec * light_specular;

  vec3 result = (A + D + S);
  return result;
}

vec3 CalcDirLight(DirectionalLight light, vec3 N, vec3 V) {
  return CalcPhong(light.ambient, light.diffuse, light.specular, N, normalize(-light.direction), V);
}

vec3 CalcPointLight(PointLight light, vec3 N, vec3 position, vec3 V) {
  vec3 L = normalize(light.position - position);
  vec3 phong_result = CalcPhong(light.ambient, light.diffuse, light.specular, N, L, V);

  return phong_result;// *light.attenuation(position);

}

vec3 CalcSpotLight(SpotLight light, vec3 N, vec3 position, vec3 V) {
  vec3 L = normalize(light.position - position);
  vec3 phong_result = CalcPhong(light.ambient, light.diffuse, light.specular, N, L, V);

  return phong_result * light.intensity(position);
}

