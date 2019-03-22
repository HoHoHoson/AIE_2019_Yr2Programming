#version 410

layout( location = 0 ) in vec4 Position;
layout( location = 1 ) in vec4 Normal;

out vec3 vNormal;

uniform mat4 ProjectionViewModel;

uniform mat3 NormalMatrix;

void main() 
{
	vNormal = NormalMatrix * Normal.xyz;
	gl_Position = ProjectionViewModel * Position;
}