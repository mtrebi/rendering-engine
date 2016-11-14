#version 330 core
struct Material {
	sampler2D diffuse;	
	sampler2D specular;	
    float shininess;
}; 

struct Light {
	vec3 color;
	//vec3 position;
	vec3 direction;
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
in vec2 TexCoords;

out vec4 color;


void main()
{
	// Ambient
	//vec3 ambientComponent = vec3(texture(material.diffuse, TexCoords)) * light.Ka;
	vec3 ambientComponent = vec3(texture(material.diffuse, TexCoords)) * light.Ka;

	// Diffuse
	vec3 normal = normalize(Normal);
	//vec3 lightVector = normalize(light.position - Position);
	vec3 lightVector = normalize(-light.direction);
	//vec3 diffuseComponent = vec3(texture(material.diffuse, TexCoords)) * light.Kd * light.color * (max(dot(lightVector, normal), 0.0f)) ;
	vec3 diffuseComponent = vec3(texture(material.diffuse, TexCoords)) * light.Kd * light.color * (max(dot(lightVector, normal), 0.0f)) ;

	// Specular
	vec3 viewDir = normalize(viewPos - Position);
	vec3 reflectedDir = reflect(-lightVector, normal);
	vec3 specularComponent = vec3(texture(material.specular, TexCoords)) * light.Ks * light.color * pow(max(dot(reflectedDir, viewDir), 0.0f), material.shininess);

	vec3 phong = (ambientComponent + diffuseComponent + specularComponent);

 	color = vec4(phong, 1.0f);
}