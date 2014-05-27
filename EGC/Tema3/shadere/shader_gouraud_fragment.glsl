#version 330
layout(location = 0) out vec4 out_color;

uniform vec4 object_color;

in float light;
in vec3 vertex_to_fragment_color;

void main(){

	out_color = object_color + vec4(vertex_to_fragment_color,1) + vec4(light,light, light,1);

}