--vertex
#version 330 core

in  vec3 position;
in  vec3 color;
out vec3 clr;

void main(void)
{
    gl_Position = vec4(position, 1.0);
    clr         = color;
}

--fragment
#version 330 core

in  vec3 clr;
out vec4 color;

void main(void)
{
	color = vec4(clr, 1.0);
}
