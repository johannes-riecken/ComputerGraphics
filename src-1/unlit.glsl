-- vertex

#version 330 core

uniform mat4 proj;
uniform mat4 mv;

layout(location = 0) in vec3 pos;
layout(location = 2) in vec3 normal;

void main(void)
{
	vec4 	p = mv * vec4 ( pos, 1.0 );
	
	gl_Position  = proj * p;
}

-- fragment
#version 330 core

out vec4 color;

void main(void)
{
	color = vec4 ( 0.0, 1.0, 0.0, 1.0 );		// green color
}
