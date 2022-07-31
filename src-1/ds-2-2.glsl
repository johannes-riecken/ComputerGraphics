
-- vertex

#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

uniform mat4 mv;
uniform mat4 proj;

out vec4 pp;
out vec2 tex;

void main(void)
{
  tex         = texCoord;
  pp           = mv * vec4 ( pos.xyz, 1.0 );
  gl_Position = proj * pp;
}

-- fragment

#version 330 core

uniform sampler2D posMap;
uniform sampler2D normalMap;
uniform sampler2D diffMap;
uniform vec4      light;

in  vec4 pp;
in  vec2 tex;
out vec4 color;

void main(void)
{
	float	z  = texture ( posMap,    tex ).z;
	vec3	p  = pp.xyz * z / pp.z;
	vec3	n  = texture( normalMap, tex ).xyz;
	vec4    c  = texture( diffMap, tex );
	vec3	l  = normalize ( light.xyz - p );
	vec3	v  = normalize ( -p );
	vec3	h  = normalize ( l + v );
	float	diff = max ( 0.2, max( 0.0, dot ( l, n ) ) );
	float	spec = pow ( max ( 0.0, dot ( h, n ) ), c.a );
	
	color = vec4 ( diff*c.rgb + vec3( spec ), 1.0 );
}
