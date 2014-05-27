#version 330
layout(location = 0) out vec4 out_color;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform float material_kd;
uniform float material_ks;

uniform sampler2D textura;

in vec3 world_pos;
in vec3 world_normal;
in vec2 texcoord;

void main(){

	float light=0;

	float ambient, difuz, specular;
	int primeste_lumina;
	vec3 L, V;

	L = normalize ( light_position - world_pos);
	V  = normalize  (eye_position - world_pos);

	ambient = material_kd * 0.2;

	difuz = material_kd * max(0, dot(world_normal,L)) ;

	if( dot(world_normal,L) > 0)
		primeste_lumina = 1;
	else
		primeste_lumina = 0;

	vec3 N = normalize(world_normal);
	vec3 R = reflect(-L,N); 
	specular = material_ks * primeste_lumina * pow((max(dot(R, V), 0)), material_shininess);

	light = ambient + difuz + specular;

	vec3 tex = texture(textura, texcoord).xyz;

	out_color = vec4(tex, 1) + vec4(light,light, light,1);
}