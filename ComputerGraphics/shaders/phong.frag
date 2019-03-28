// Phong fragment shader

#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform vec3 Ka; 
uniform vec3 Kd; 
uniform vec3 Ks; 
uniform float specularPower;

struct Light
{
	vec3 direction;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 Ia;

uniform vec3 Id; 
uniform vec3 Is; 
uniform vec3 LightDirection;

uniform vec3 CameraPosition;
uniform sampler2D DiffuseTexture;

out vec4 FragColour;

void main() 
{

	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);

	float lambertTerm = max( 0, min( 1, dot( N, -L ) ) );

	vec3 V = normalize(CameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );

	float specularTerm = pow( max( 0, dot( R, V ) ), specularPower );

	vec3 ambient = Ia * Ka;
	vec3 diffuse = Id * Kd * lambertTerm;
	vec3 specular = Is * Ks * specularTerm;
	FragColour = vec4( ambient + diffuse + specular, 1) * texture(DiffuseTexture, vTexCoord);
}

vec3 CalcDirectLight(in Light light, in vec3 N, )
{

}
