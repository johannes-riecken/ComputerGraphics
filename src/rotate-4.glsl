--vertex
#version 330 core

layout ( location = 0 ) in vec3 pos;
layout ( location = 1 ) in vec2 tex;
out vec3  p;

uniform mat4 proj;
uniform mat4 mv;

void main(void)
{
    gl_Position = proj * mv * vec4(pos, 1.0);
	p           = pos;
}

--fragment
#version 330 core

uniform sampler3D image;
in  vec3 p;
out vec4 color;

void main(void)
{
	color = texture ( image, p );
}
