#version 330

layout(location = 0) in vec3 in_position;		
layout(location = 1) in vec3 in_color;		

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform float offset;

out vec3 vertex_to_fragment_color;

void main(){

	vec3 copy_position = in_position;
	copy_position[2] = 4*sin((((in_position[0]-offset)*15)/360.0f)*3.141592654*2.0f);

	vertex_to_fragment_color = in_color;

	gl_Position = projection_matrix*view_matrix*model_matrix*vec4(copy_position,1); 
}
