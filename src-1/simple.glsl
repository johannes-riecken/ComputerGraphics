--vertex
#version 330 core

in vec3 position;

void main(void)
{
	gl_Position = vec4(position, 1.0);
}

--fragment
#version 330 core

out vec4 color;

void main(void)
{
	color = vec4(1.0, 0.0, 0.0, 1.0);
}
