#version 330

layout(location = 0) in vec3 start_position;
layout(location = 1) in vec3 end_position;
layout(location = 2) in vec3 start_normal;
layout(location = 3) in vec3 end_normal;
layout(location = 4) in vec2 start_texcoord;
layout(location = 5) in vec2 end_texcoord;

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform float time;

out vec3 world_pos;
out vec3 world_normal;
out vec2 texcoord;

void main(){

	vec3 inter_position = start_position*(1-time) + end_position*time;
	vec3 inter_normal = start_normal*(1-time) + end_normal*time;
	vec2 inter_texcoord = start_texcoord*(1-time) + end_texcoord*time;

	world_pos = (model_matrix * vec4(inter_position,1)).xyz;
	world_normal = normalize(  mat3(model_matrix) *  inter_normal );
	texcoord = inter_texcoord;

	gl_Position = projection_matrix * view_matrix * model_matrix * vec4( inter_position, 1); 

}
