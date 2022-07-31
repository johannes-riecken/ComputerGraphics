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

in vec3 v;
in vec3 l;
in vec3 n;

out vec4 color;

void main( void )
{
  vec4  clr  = vec4 ( 0.7, 0.1, 0.1, 1.0 );
  float roughness = 0.58;
  
  vec3  l2  = normalize( l );
  vec3  n2  = normalize( n );
  vec3  v2  = normalize( v );
  float nl = dot( n2, l2 );
  float nv = dot( n2, v2 );
  vec3  lProj    = normalize(l2 - n2 * nl);
  vec3  vProj    = normalize(v2 - n2 * nv);
  float cx       = max(dot( lProj, vProj), 0.0);
  float cosAlpha = max(nl, nv);
  float cosBeta  = min(nl, nv);
  float ca2      = cosAlpha*cosAlpha;
  float cb2      = cosBeta * cosBeta;   
  float dx       = sqrt((1.0 - ca2)*(1.0 - cb2)/
                        (cb2+0.001));
  float r2       = roughness * roughness;
  float a        = 1.0 - 0.5 * r2 / (r2 + 0.33);
  float b        = 0.45 * r2 / ( r2 + 0.09 );  
   
  color = max(nl, 0.0) * clr * (a + b * cx * dx);
}
