
#version 430

in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec2 tc;
in vec3 originalVertex;
out vec4 fragColor;

struct PositionalLight
{	vec4 ambient;  
	vec4 diffuse;  
	vec4 specular;  
	vec3 position;
};

struct Material
{	vec4 ambient;  
	vec4 diffuse;  
	vec4 specular;  
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;	 
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
layout (binding=0) uniform sampler2D s;

void main(void)
{	// normalize the light, normal, and view vectors:
	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);

	float a = 0.25;		// controls depth of bumps
	float b = 100.0;	// controls width of bumps
	float x = originalVertex.x;
	float y = originalVertex.y;
	float z = originalVertex.z;
	N.x = varyingNormal.x + a*sin(b*x);
	N.y = varyingNormal.y + a*sin(b*y);
	N.z = varyingNormal.z + a*sin(b*z);
	N = normalize(N);

	// compute light reflection vector, with respect N:
	vec3 R = normalize(reflect(-L, N));
	
	// get the angle between the light and surface normal:
	float cosTheta = dot(L,N);
	
	// angle between the view vector and reflected light:
	float cosPhi = dot(V,R);

	// compute ADS contributions (per pixel):
	vec3 ambient = (((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz);
	//vec3 ambient = globalAmbient*texture(s,tc)*(0.5*+0.5*N*U)+light.ambient*texture(s,tc);//did not work why? check U
	vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta,0.0);
	vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi,0.0), material.shininess);
	
	fragColor = vec4((ambient + diffuse + specular), 1.0)*0.2+texture(s,tc)*0.5;
}
