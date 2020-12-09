#version 330 core
in vec2 texCoords;
in vec3 normal;
uniform sampler2D texture0;

void main()
{
	gl_FragColor = texture(texture0, texCoords*20);
	//gl_FragColor = vec4(1,0,0,1);
}
