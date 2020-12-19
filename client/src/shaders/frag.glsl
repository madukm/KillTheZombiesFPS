#version 330 core
in vec2 texCoords;
in vec3 normal;

uniform sampler2D texture0;
uniform vec3 color;
uniform bool onlyColor;
uniform bool processLight;// Enable/disable light (important when drawing UI)

// Global Sun
vec3 sunPos = vec3(0,10,0); // Sun position
vec3 sunDir = normalize(vec3(2,10,4)); // Sun direction (to the world center)
vec3 sunColor = vec3(1.0, 1.0, 0.9);
float ksun = 0.5;

float ka = 0.5;

void main()
{
	vec4 mainColor;
	if(onlyColor)
		mainColor = vec4(color,1);
	else
		mainColor = texture(texture0, texCoords); 

	if(processLight)
	{
		vec3 n = normalize(normal);
		// Calculating directional light (Sun)
		vec3 directional = ksun*sunColor*max(dot(sunDir, n), 0);
		vec3 ambient = ka*mainColor.xyz;
		mainColor *= vec4(directional+ambient,1);
	}

	gl_FragColor = mainColor;
}
