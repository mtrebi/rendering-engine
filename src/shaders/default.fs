#version 330 core
out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 Position;

void main()
{
	const float Ka = 0.1f;
	const float Kd = 0.4f;
	const float Ks = 0.4f;
	const float shininess = 32.0f;

	// Ambient
	vec3 ambientComponent = Ka * lightColor;

	// Diffuse
	vec3 normal = normalize(Normal);
	vec3 lightVector = normalize(lightPos - Position);
	vec3 diffuseComponent = Kd * (max(dot(lightVector, normal), 0.0f)) * lightColor;

	// Specular
	vec3 viewDir = normalize(viewPos - Position);
	vec3 reflectedDir = reflect(-lightVector, normal);
	vec3 specularComponent = Ks * pow(max(dot(reflectedDir, viewDir), 0.0f), 32) * lightColor;

	vec3 phong = (ambientComponent + diffuseComponent + specularComponent) * objectColor;

 	color = vec4(phong, 1.0f);
}