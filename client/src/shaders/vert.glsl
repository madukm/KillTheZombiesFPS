#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

out vec2 texCoords;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	texCoords = inTexCoords;
	normal = inNormal;

	gl_Position = projection*view*model*vec4(position, 1.0);
}
