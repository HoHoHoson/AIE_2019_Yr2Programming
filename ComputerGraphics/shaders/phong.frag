#version 410

in vec3 vNormal;

uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;

uniform vec3 LightDirection;

out vec4 FragColour;

void main()
{
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);
	
	float lambertTerm = max(0, min(1, dot(N, -L )));
	
	vec3 diffuse = Id * lambertTerm;
	
	FragColour = vec4(diffuse, 1);
}