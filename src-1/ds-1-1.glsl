
-- vertex

#version 330 core

uniform mat4 mv;
uniform mat4 proj;
uniform mat3 nm;

layout ( location = 0 ) in vec3 pos;
layout ( location = 1 ) in vec2 texCoord;
layout ( location = 2 ) in vec3 normal;

out vec2 tex;
out vec3 n;
out vec4 p;

void main(void)
{
	tex         = texCoord;
	n           = nm * normal;
	p           = mv * vec4 ( pos, 1.0 );
	gl_Position = proj * p;
}

-- fragment

#version 330 core

//uniform sampler2D	image;

in  vec2 tex;
in  vec3 n;
in  vec4 p;
out vec4 color [2];

void main(void)
{
    color [0] = vec4 ( p.xyz, 1.0 );
	color [1] = vec4 ( n, 1.0 );
}
