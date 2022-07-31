
-- vertex

#version 330 core

layout(location = 0) in vec4 pos;

out vec2 tex;

void main(void)
{
  tex         = pos.zw;
  gl_Position = vec4 ( pos.xy, 0.0, 1.0 );
}

-- fragment

#version 330 core

uniform sampler2D posMap;
uniform sampler2D normalMap;
uniform vec4      light;

in  vec2 tex;
out vec4 color;

void main(void)
{
//	color = 0.5 * ( vec4 ( 1.0 ) + texture( normalMap, tex ) );

	vec3	p  = texture( posMap,    tex ).xyz;
	vec3	n  = texture( normalMap, tex ).xyz;
	vec3	l  = normalize ( light.xyz - p );
	vec3	v  = normalize ( -p );
	vec3	h  = normalize ( l + v );
	float	diff = max ( 0.2, dot ( l, n ) );
	float	spec = pow ( max ( 0.0, dot ( h, n ) ), 40.0 );
	
	color = vec4 ( vec3 ( diff + spec ), 1.0 );
}
