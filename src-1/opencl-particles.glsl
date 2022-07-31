
-- vertex

#version 330 core

uniform mat4 mv;

layout(location = 0) in vec3 pos;

void main(void)
{
	gl_Position = mv * vec4 ( pos, 1.0 );
}

-- fragment

#version 330 core

out vec4 color;

void main(void)
{
	color = vec4 ( 1.0, 1.0, 0.0, 1.0 );
}
