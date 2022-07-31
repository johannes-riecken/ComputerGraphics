-- vertex

#version 330 core

uniform mat4 proj;
uniform mat4 mv;
uniform mat3 nm;
uniform	vec3 eye;
uniform vec3 light;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 normal;

out vec3 n;
out vec3 v;
out vec3 l;

void main(void)
{
	vec4	p = mv * vec4 ( pos, 1.0 );

	v           = normalize( eye - p.xyz );
	n           = normalize( nm * normal );
	l           = normalize( light - p.xyz );
	gl_Position = proj * p;
}

-- fragment

#version 330 core

uniform sampler2D toonMap;

in vec3 n;
in vec3 v;
in vec3 l;

out vec4 color;

void main (void)
{
	const vec4	diffColor = vec4 ( 0.5, 0.0, 0.0, 1.0 );

	vec3	n2   = normalize ( n );
	vec3	l2   = normalize ( l );
	vec3	v2   = normalize ( v );
	float	diff = 0.2 + max ( dot ( n2, l2 ), 0.0 );

	color = texture( toonMap, vec2 ( diff, gl_FragCoord.z ) );
}
