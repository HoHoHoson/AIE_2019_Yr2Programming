// Phong fragment shader

#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform vec3 Ka; 
uniform vec3 Kd; 
uniform vec3 Ks; 
uniform float specularPower;

uniform vec3 CameraPosition;
uniform sampler2D DiffuseTexture;

uniform vec3 Ia;

uniform vec3 Id1; 
uniform vec3 Is1; 
uniform vec3 LightDirection1;

uniform vec3 Id2;
uniform vec3 Is2;
uniform vec3 LightDirection2;

out vec4 FragColour;

vec3 CalcDirectLight(in vec3 dir, in vec3 Id, in vec3 Is, in vec3 n, in vec3 v)
{
	vec3 L = normalize(dir);
	
	float lambertTerm = max( 0, min( 1, dot( n, -L ) ) );
	
	vec3 R = reflect( L, n );
	
	float specularTerm = pow( max( 0, dot( R, v ) ), specularPower );
	
	vec3 ambient = Ia * Ka;
	vec3 diffuse = Id * Kd * lambertTerm;
	vec3 specular = Is * Ks * specularTerm;
	
	vec3 dirLight = (ambient + diffuse + specular);
	
	return dirLight;
}

void main() 
{
	vec3 TotalLight = vec3(0, 0, 0);
	vec3 N = normalize(vNormal);
	vec3 V = normalize(CameraPosition - vPosition.xyz);

	TotalLight += CalcDirectLight(LightDirection1, Id1, Is1, N, V);
	TotalLight += CalcDirectLight(LightDirection2, Id2, Is2, N, V);
	
	FragColour = vec4(TotalLight, 1) * texture(DiffuseTexture, vTexCoord);
}


