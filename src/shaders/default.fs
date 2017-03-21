#version 330 core

in vec3 vs_normal;
in vec3 vs_position;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 camera_position;

out vec4 color;

vec3 ambient() {
  return 0.1f * light_color;
}

vec3 diffuse() {
  vec3 N = normalize(vs_normal);
  vec3 L = normalize(light_position - vs_position);
  float diffuse = max(dot(N, L), 0.0f);
  return 0.5f * diffuse * light_color;
}
vec3 specular() {
  float specularStrength = 0.5f;
  vec3 N = normalize(vs_normal);
  vec3 V = normalize(camera_position - vs_position);
  vec3 L = normalize(light_position - vs_position);
  vec3 R = reflect(-L, N);
  float spec = pow(max(dot(V, R), 0.0), 1);
  vec3 specular = specularStrength * spec * light_color;
  return specular;
}

/*
vec3 specular() {
  vec3 N = normalize(vs_normal);
  vec3 V = normalize(camera_position - vs_position);
  vec3 L = normalize(light_position - vs_position);
  //vec3 H = normalize(L + V);
  vec3 R = reflect(-L, N);
  float specular = pow(max(dot(V, R), 0.0f), 32.0f);
  //float specular = pow(max(0.0f, dot(H, N)), 32);

  return 0.5f * specular * light_color;
}
*/

void main() {
  
  // Ambient
  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * light_color;

  // Diffuse 
  vec3 norm = normalize(vs_normal);
  vec3 lightDir = normalize(light_position - vs_position);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * light_color;

  // Specular
  float specularStrength = 0.5f;
  vec3 viewDir = normalize(camera_position - vs_position);
  vec3 reflectDir = normalize(reflect(-lightDir, norm));
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
  vec3 specular = specularStrength * spec * light_color;

  vec3 result = (ambient +diffuse + specular) * object_color;
  color = vec4(result, 1.0f);
  
}