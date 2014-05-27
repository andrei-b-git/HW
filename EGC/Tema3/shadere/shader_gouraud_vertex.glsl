#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_color;

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform float material_kd;
uniform float material_ks;

out float light;
out vec3 vertex_to_fragment_color;

void main(){

	vec3 world_pos = (model_matrix * vec4(in_position,1)).xyz;
	vec3 world_normal = normalize(  mat3(model_matrix) *  in_normal );

	float ambient, difuz, specular;
	int primeste_lumina;
	vec3 L, V, H;

	L = normalize (light_position - world_pos);
	V  = normalize (eye_position - world_pos);
	H = normalize (L + V);

	ambient = material_kd * 0.2;

	difuz = material_kd * max(0, dot(world_normal,L)) ;

	if( dot(world_normal,L) > 0)
		primeste_lumina = 1;
	else
		primeste_lumina = 0;

	specular = material_ks * primeste_lumina * pow((max(dot(world_normal, H), 0)), material_shininess);

	light = ambient + difuz + specular;

	vertex_to_fragment_color = in_color;

	gl_Position = projection_matrix*view_matrix*model_matrix*vec4(in_position,1); 
}
