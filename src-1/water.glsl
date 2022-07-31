--vertex
#version 330 core

layout ( location = 0 ) in vec3 pos;
layout ( location = 2 ) in vec3 normal;
layout ( location = 3 ) in vec3 tangent;
layout ( location = 4 ) in vec3 binormal;

out vec3 v;					// vector to the eye
out vec3 pp;

uniform mat4 proj;
uniform mat4 mv;
uniform mat3 nm;
uniform vec3 eye;

void main(void)
{
	vec4 	p = mv * vec4 ( pos, 1.0 );
	vec3	n = normalize ( nm * normal );
	vec3	t = normalize ( nm * tangent );
	vec3	b = normalize ( nm * binormal );
	
	gl_Position  = proj * p;
	v            = normalize ( eye - p.xyz );
	v            = vec3 ( dot ( v, t ), dot ( v, b ), dot ( v, n ) );
	pp           = pos;
}

--fragment
#version 330 core

uniform samplerCube	image;
uniform sampler3D	noiseMap;
uniform float       time;

in  vec3 v;
in  vec3 pp;
out vec4 color;

vec4 turbulence ( in vec3 t )
{
  return 2.0*texture(noiseMap, t)-vec4(1.0)+
         1.0*texture(noiseMap, 2.77*t)-vec4(0.5)+
         0.5*texture(noiseMap, 2.77*2.77*t)-vec4(0.25);
}

void main(void)
{
	const vec4	c2 = 2.2 * vec4 ( 0.03, 0.15, 0.125, 1.0 );		// green-blue
	const vec4	c1 = 1.6 * vec4 ( 0.03, 0.22,  0.07, 1.0 );		// green
	const vec3  dp = vec3 ( 0.01534, 0.0141231, 0.0272312 );
	
	vec4  t  = turbulence ( 0.2*pp.xyz + time * dp );
	vec3  n2 = normalize  ( vec3 ( 0.0, 0.0, 1.0 ) + 0.2 * t.xyz );
	vec3  v2 = normalize  ( v );
	vec3  r  = reflect    ( v2, n2 ).xzy;
	float nv = dot ( n2, v2 );
	vec4  c  = mix ( c2, c1, nv );
	float fresnel = 1.0 - nv;//1.0 / pow ( 1.0 + nv, 8.0 );
	vec4  refl    = texture( image, vec3 (r.x, r.y, r.z ) );
	
	color = c*(1.0 - fresnel) + refl*fresnel;
}
