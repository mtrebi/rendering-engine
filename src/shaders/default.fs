#version 330 core
struct Material {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float shininess;
}; 

struct Light {
	vec3 color;
	vec3 position;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
}; 

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 Normal;
in vec3 Position;

out vec4 color;


void main()
{
	// Ambient
	vec3 ambientComponent = material.Ka * light.Ka;

	// Diffuse
	vec3 normal = normalize(Normal);
	vec3 lightVector = normalize(light.position - Position);
	vec3 diffuseComponent = material.Kd * light.Kd * light.color * (max(dot(lightVector, normal), 0.0f)) ;

	// Specular
	vec3 viewDir = normalize(viewPos - Position);
	vec3 reflectedDir = reflect(-lightVector, normal);
	vec3 specularComponent = material.Ks * light.Ks * light.color * pow(max(dot(reflectedDir, viewDir), 0.0f), material.shininess);

	vec3 phong = (ambientComponent + diffuseComponent + specularComponent) * objectColor;

 	color = vec4(phong, 1.0f);
}