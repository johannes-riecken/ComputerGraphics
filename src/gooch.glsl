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
out	vec3 h;

void main(void)
{
	vec4	p = mv * vec4 ( pos, 1.0 );

	v           = normalize( eye - p.xyz );
	n           = normalize( nm * normal );
	l           = normalize( light - p.xyz );
	h           = normalize( l + v );	
	gl_Position = proj * p;
}

-- fragment

#version 330 core

in vec3 n;
in vec3 v;
in vec3 l;
in vec3 h;

out vec4 color;

void main (void)
{
	const vec4  diffColor = vec4 ( 0.6, 0.6, 0.6, 1.0 );
	const vec4	specColor = vec4 ( 1.0 );
	const float alpha     = 0.45;
	const float beta      = 0.45;
	
	vec4  cool = diffColor + vec4 ( 0.0, 0.0, alpha, 0.0 );
	vec4  warm = diffColor + vec4 ( beta, beta, 0.0, 0.0 );
	
	vec3  n2 = normalize ( n );
	vec3  l2 = normalize ( l );
	vec3  h2 = normalize ( h );
	float nl = dot ( n2, l2 );
	float spec = pow ( max ( dot ( n2, h2 ), 0.0 ), 70.0 );
	vec4  diff = 0.5*(1.0+nl)*cool + 0.5*(1.0-nl)*warm;
		
	color = diff + spec * specColor;
}
