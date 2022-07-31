-- vertex

#version 330 core

uniform mat4 proj;
uniform mat4 mv;
uniform mat3 nm;
uniform	vec3 eye;		// eye position
uniform vec3 light;     // light position

layout(location = 0) in vec3 pos;
layout(location = 2) in vec3 normal;

out vec3  n;
out vec3  v;
out vec3  l;

void main(void)
{
	vec4 	p = mv * vec4 ( pos, 1.0 );
	
	gl_Position  = proj * p;
	n            = normalize ( nm * normal );
	v            = normalize ( eye - p.xyz );					// vector to the eye
	l            = normalize ( light - p.xyz );
}

-- fragment
#version 330 core

in	vec3 n;
in	vec3 v;
in	vec3 l;

out vec4 color;

void main(void)
{
	vec3  n2   = normalize ( n );
	vec3  l2   = normalize ( l );
	float nl   = dot(n2, l2);
	vec3  r    = (2.0*nl) * n2 - l2;
	float diff = max ( nl, 0.0 );
	float spec = pow ( max ( dot ( n2, r ), 0.0 ), 50.0 );
	vec4  clr  = vec4 ( 0.7, 0.1, 0.1, 1.0 );
	float ka   = 0.2;
	float kd   = 0.8;
	float ks   = 0.5;

	color = (ka + kd*diff)*clr + ks*vec4(spec);
}
