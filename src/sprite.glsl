--vertex
#version 330 core

layout ( location = 0 ) in vec3 pos;


uniform mat4 proj;
uniform mat4 mv;

void main(void)
{
	gl_Position  = proj * mv * vec4 ( pos, 1.0 );
	gl_PointSize = 15;
}

--fragment
#version 330 core

uniform sampler2D	image;

out vec4 color;

void main(void)
{
	color = vec4 ( texture ( image, gl_PointCoord.xy ).r );
}
