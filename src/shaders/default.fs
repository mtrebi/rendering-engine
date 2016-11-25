#version 330 core
struct Material {
	sampler2D diffuse;	
	sampler2D specular;	
    float shininess;
}; 

struct DirectionalLight {
	vec3 direction;
	// Phong
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
};

struct PointLight {
	vec3 position;
	// Phong
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
    // Attenuation
    float Kc;
    float Kl;
    float Kq;
};

struct SpotLight{
	vec3 direction;
	vec3 position;
	// Radius
	float innerCutOff;
	float outerCutOff;
	// Phong
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;	
    // Attenuation
    float Kc;
    float Kl;
    float Kq;
};


uniform vec3 objectColor;
uniform vec3 viewPos;
uniform Material material;

// Lights
uniform DirectionalLight directionalLight;
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

in vec3 Normal;
in vec3 FragPosition;
in vec2 TexCoords;

out vec4 color;

// Function prototypes
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 view);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 view);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 view);

float CalculatePointLightAttenuation(float Kc, float Kl, float Kq, float distance);

void main()
{
	vec3 n_normal = normalize(Normal);
	vec3 n_view = normalize(viewPos - FragPosition);

	vec3 directionalContribution;//CalculateDirectionalLight(directionalLight, n_normal, n_view);
	vec3 spotLightContribution = CalculateSpotLight(spotLight, n_normal, FragPosition, n_view);

	vec3 pointContribution;
	for (int l = 0; l < NR_POINT_LIGHTS; ++l){
		pointContribution+= CalculatePointLight(pointLights[l], n_normal, FragPosition, n_view);
	}


	vec3 result = directionalContribution + pointContribution + spotLightContribution;

	color = vec4(result, 1.0);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 view){
	vec3 n_light = normalize(-light.direction);
	//vec3 reflected = reflect(-n_light, normal);
	vec3 halfway = normalize(-light.direction + view)


	vec3 ambientComponent 	= vec3(texture(material.diffuse, 	TexCoords)) * light.Ka;
	vec3 diffuseComponent 	= vec3(texture(material.diffuse, 	TexCoords)) * light.Kd * (max(dot(n_light, normal), 0.0f)) ;
	vec3 specularComponent 	= vec3(texture(material.specular, 	TexCoords)) * light.Ks * pow(max(dot(halfway, view), 0.0f), material.shininess);

	return (ambientComponent + diffuseComponent + specularComponent);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 view){
	vec3 n_light = normalize(light.position - fragPos);
	//vec3 reflected = reflect(-n_light, normal);
	vec3 halfway = normalize(-light.direction + view)


	vec3 ambientComponent 	= vec3(texture(material.diffuse, 	TexCoords)) * light.Ka;
	vec3 diffuseComponent 	= vec3(texture(material.diffuse, 	TexCoords)) * light.Kd * (max(dot(n_light, normal), 0.0f)) ;
	vec3 specularComponent 	= vec3(texture(material.specular, 	TexCoords)) * light.Ks * pow(max(dot(halfway, view), 0.0f), material.shininess);

	float distance = length(light.position - fragPos);

	return (ambientComponent + diffuseComponent + specularComponent) * CalculatePointLightAttenuation(light.Kc, light.Kl, light.Kq, distance);
}

float CalculatePointLightAttenuation(float Kc, float Kl, float Kq, float distance){
	return 1.0f / ((Kc + Kl * distance) + (Kq * pow(distance,2)));
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 view){
	vec3 n_light = normalize(light.position - fragPos);
	//vec3 reflected = reflect(-n_light, normal);
	vec3 halfway = normalize(-light.direction + view)

	vec3 spotDirection = normalize(fragPos - light.position);
	float theta = dot(light.direction, spotDirection);
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// Phong
	vec3 ambientComponent 	= vec3(texture(material.diffuse, 	TexCoords)) * light.Ka;
	vec3 diffuseComponent 	= vec3(texture(material.diffuse, 	TexCoords)) * light.Kd * (max(dot(n_light, normal), 0.0f)) ;
	vec3 specularComponent 	= vec3(texture(material.specular, 	TexCoords)) * light.Ks * pow(max(dot(halfway, view), 0.0f), material.shininess);

	return (ambientComponent + (diffuseComponent + specularComponent) * intensity) * CalculatePointLightAttenuation(light.Kc, light.Kl, light.Kq, length(light.position - fragPos));
}
