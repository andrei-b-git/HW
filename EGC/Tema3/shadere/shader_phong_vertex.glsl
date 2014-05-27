#version 330

layout(location = 0) in vec3 in_position;	
layout(location = 1) in vec3 in_normal;	
layout(location = 2) in vec3 in_color;		

uniform mat4 model_matrix, view_matrix, projection_matrix;

out vec3 world_pos;
out vec3 world_normal;
out vec3 vertex_to_fragment_color;

void main(){

	world_pos = (model_matrix * vec4(in_position,1)).xyz;
	world_normal = normalize(  mat3(model_matrix) *  in_normal );

	vertex_to_fragment_color = in_color;

	gl_Position = projection_matrix*view_matrix*model_matrix*vec4(in_position,1);

}
