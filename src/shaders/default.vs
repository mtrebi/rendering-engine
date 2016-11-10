#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourPosition;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0f);
    ourPosition = position;
	// We swap the y-axis by substracing our coordinates from 1. This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
	TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}
