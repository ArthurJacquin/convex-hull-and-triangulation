#version 420
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec3 a_normal;

out vec4 v_color;
out vec3 v_normal;
out vec3 v_fragPos;

uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

void main(void) 
{ 
	v_normal = a_normal;
	v_color = vec4(a_color, 1.0); 
	v_fragPos = a_position;

	gl_Position = u_projectionMatrix * u_viewMatrix * vec4(a_position , 1.0);
	gl_PointSize = 5.0;
}