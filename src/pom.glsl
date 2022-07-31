-- vertex

#version 330 core

uniform mat4 proj;
uniform mat4 mv;
uniform mat3 nm;
uniform	vec3 eye;		// eye position
uniform vec3 light;     // light position

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec2 texCoord;
layout ( location = 2 ) in vec3 normal;
layout ( location = 3 ) in vec3 tangent;
layout ( location = 4 ) in vec3 binormal;

out vec3  n;
out vec3  v;
out vec3  l;
out	vec3  h;
out vec2  tex;

void main(void)
{
	vec4 	p = mv * vec4 ( position, 1.0 );
	vec3	t = normalize ( nm * tangent );
	vec3	b = normalize ( nm * binormal );
	
	gl_Position  = proj * p;
	n            = normalize ( nm * normal );
	v            = normalize ( eye - p.xyz );					// vector to the eye
	l            = normalize ( light - p.xyz );
	h            = normalize ( l + v );	
	v            = vec3 ( dot ( v, t ), dot ( v, b ), dot ( v, n ) );
	l            = vec3 ( dot ( l, t ), dot ( l, b ), dot ( l, n ) );
	h            = vec3 ( dot ( h, t ), dot ( h, b ), dot ( h, n ) );
	tex          = texCoord;
}

-- fragment
#version 330 core

in	vec3 n;
in	vec3 v;
in	vec3 l;
in	vec3 h;
in  vec2 tex;
out vec4 color;

uniform sampler2D decalMap;
uniform sampler2D heightMap;
uniform	float	  scale;

void main (void)
{

	const float numSteps  = 10.0;
	const float	specExp   = 80.0;

	float	step   = 1.0 / numSteps;
	vec2	dt     = v.xy * scale / ( numSteps * v.z );	// adjustment for one layer
	float	height = 1.0;								// height of the layer
	vec2	t      = tex;								// our initial guess
	float	hh     = 1.0-texture ( heightMap, tex ).a;	// get height

	while ( hh < height )
	{
		height -= step;
		t      += dt;
		hh      = 1.0-texture ( heightMap, t ).a;
	}
												// now find point via linear interpolation
	vec2	prev   = t - dt;					// previous point
	float	hPrev  = 1.0-texture ( heightMap, prev ).a - (height + step);
	float	hCur   = hh - height;
	float	weight = hCur / (hCur - hPrev );
	
	t = weight * prev + (1.0 - weight) * t;
	
	vec3	clr   = texture   ( decalMap, t ).rgb;
	vec3	n     = normalize ( texture ( heightMap, t ).rgb * 2.0 - vec3 ( 1.0 ) );
	float	diff  = max ( dot ( n, normalize ( l ) ), 0.0 );
	float	spec  = pow ( max ( dot ( n, normalize ( h ) ), 0.0 ), specExp );

	color = vec4 ( clr * diff + vec3 ( 0.7 * spec ), 1.0 );
}
