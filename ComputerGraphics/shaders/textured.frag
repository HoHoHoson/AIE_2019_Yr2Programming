#version 410

in vec2 vTexCoord;

out vec4 FragColour;

uniform sampler2D diffuseTexture;

void main() 
{
	FragColour = texture( diffuseTexture, vTexCoord );
}