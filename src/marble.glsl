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
out vec3 pp;

void main(void)
{
	vec4	p = mv * vec4 ( pos, 1.0 );

	v           = normalize ( eye - p.xyz );
	n           = normalize ( nm * normal );
	l            = normalize ( light - p.xyz );
	h            = normalize ( l + v );	
	pp          = pos.xyz;
	gl_Position = proj * p;
}

-- fragment

#version 330 core

uniform	float		scale;
uniform sampler3D 	noiseMap;

in vec3 n;
in vec3 v;
in vec3 l;
in vec3 h;
in vec3 pp;

out vec4 color;

vec4 turbulence ( in vec3 t )
{
  return 2.0*abs( texture(noiseMap, t)-vec4(0.5))+
         1.0*abs( texture(noiseMap, 2.0*t)-vec4(0.5))+
         0.5*abs( texture(noiseMap, 4.0*t)-vec4(0.5));
}


void main (void)
{
	const vec4	c1 = vec4 ( 0.7, 0.7, 0.7, 1.0 );
	const vec4	c2 = vec4 ( 0.0, 0.1, 1.0, 1.0 );
	const vec4	specColor = vec4 ( 0.7, 0.7, 0.0, 1.0 );
	const float	specPower = 70.0;

	vec4	ns = turbulence ( 0.1*pp );
	float	f  = 0.5*(1.0+sin(pp.x + scale*ns.x));
	vec4	c  = mix( c1, c2, f*f );

	vec3	n2   = normalize ( n );
	vec3	l2   = normalize ( l );
	vec3	h2   = normalize ( h );
	vec4	diff = vec4 ( max ( dot ( n2, l2 ), 0.0 ) + 0.3 );
	vec4	spec = specColor * pow ( max ( dot ( n2, h2 ), 0.0 ), specPower ) * (1.0-f);
		
	color = diff * c + spec;
}
